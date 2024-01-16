import argparse
import warnings
from collections import OrderedDict
from flwr.common.logger import log
from logging import WARNING, INFO

import flwr as fl
import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.utils.data import DataLoader, Dataset
from tqdm import tqdm
import pandas as pd
from flwr.common.logger import configure

configure(identifier="my_iris_app", filename="logs_fed_avg-r-50_niid.log")

# #############################################################################
# 1. Regular PyTorch pipeline: nn.Module, train, test, and DataLoader
# #############################################################################

warnings.filterwarnings("ignore", category=UserWarning)
DEVICE = torch.device("cpu")


class CustomDataset(Dataset):
    def __init__(self, features, labels):
        self.features = torch.tensor(features, dtype=torch.float32)
        self.labels = torch.tensor(labels, dtype=torch.long)

    def __len__(self):
        return len(self.features)

    def __getitem__(self, index):
        return {"img": self.features[index], "label": self.labels[index]}


class Net(nn.Module):
    def __init__(self) -> None:
        super(Net, self).__init__()

        self.fc1 = nn.Linear(4, 2)
        self.fc2 = nn.Linear(2, 3)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        x = F.relu(self.fc1(x))
        return self.fc2(x)


def train(net, trainloader, epochs):
    """Train the model on the training set."""
    criterion = torch.nn.CrossEntropyLoss()
    optimizer = torch.optim.SGD(net.parameters(), lr=0.001)
    for _ in range(epochs):
        for batch in tqdm(trainloader, "Training"):
            images = batch["img"]
            labels = batch["label"]
            optimizer.zero_grad()
            criterion(net(images.to(DEVICE)), labels.to(DEVICE)).backward()
            optimizer.step()


def test(net, testloader):
    """Validate the model on the test set."""
    criterion = torch.nn.CrossEntropyLoss()
    correct, loss = 0, 0.0
    with torch.no_grad():
        for batch in tqdm(testloader, "Testing"):
            images = batch["img"].to(DEVICE)
            labels = batch["label"].to(DEVICE)
            outputs = net(images)
            loss += criterion(outputs, labels).item()
            correct += (torch.max(outputs.data, 1)[1] == labels).sum().item()
    accuracy = correct / len(testloader.dataset)
    return loss, accuracy


def eval_data():
    test_df = pd.read_csv(f"../../data/test_iris_norm.csv", header=None)

    test_features = test_df.values[:, :-1]
    test_labels = test_df.values[:, -1]
    # Creating custom datasets
    test_dataset = CustomDataset(features=test_features, labels=test_labels)
    testloader = DataLoader(test_dataset, batch_size=1, shuffle=False)
    return testloader


def load_data(node_id, non_iid=False):
    if not non_iid:
        train_df = pd.read_csv(
            f"../../data/train_iid_iris_norm_{node_id}.csv", header=None
        )

    else:
        train_df = pd.read_csv(
            f"../../data/train_niid_iris_norm_{node_id}.csv", header=None
        )
    val_df = pd.read_csv(f"../../data/val_iris_norm.csv", header=None)
    train_features = train_df.values[:, :-1]
    train_labels = train_df.values[:, -1]
    val_features = val_df.values[:, :-1]
    val_labels = val_df.values[:, -1]
    # Creating custom datasets
    train_dataset = CustomDataset(features=train_features, labels=train_labels)
    val_dataset = CustomDataset(features=val_features, labels=val_labels)

    trainloader = DataLoader(train_dataset, batch_size=1, shuffle=True)
    valloader = DataLoader(val_dataset, batch_size=1, shuffle=False)
    return trainloader, valloader


# #############################################################################
# 2. Federation of the pipeline with Flower
# #############################################################################

# Get node id
parser = argparse.ArgumentParser(description="Flower")
parser.add_argument(
    "--node-id",
    choices=[0, 1, 2, 3, 4],
    required=True,
    type=int,
    help="Partition of the dataset divided into 3 iid partitions created artificially.",
)
node_id = parser.parse_args().node_id

# Load model and data (simple CNN, CIFAR-10)
net = Net().to(DEVICE)
trainloader, testloader = load_data(node_id=node_id, non_iid=True)


# Define Flower client
class FlowerClient(fl.client.NumPyClient):
    def get_parameters(self, config):
        return [val.cpu().numpy() for _, val in net.state_dict().items()]

    def set_parameters(self, parameters):
        params_dict = zip(net.state_dict().keys(), parameters)
        state_dict = OrderedDict({k: torch.tensor(v) for k, v in params_dict})
        net.load_state_dict(state_dict, strict=True)

    def fit(self, parameters, config):
        self.set_parameters(parameters)
        train(net, trainloader, epochs=10)
        return self.get_parameters(config={}), len(trainloader.dataset), {}

    def evaluate(self, parameters, config):
        self.set_parameters(parameters)
        loss, accuracy = test(net, testloader)
        if config["server_round"] == 50:
            loss_final, accuracy_final = test(net, eval_data())
            log(INFO, f"Final accuracy: {accuracy_final}")
            log(INFO, f"Final loss: {loss_final}")

            # print(f"Final accuracy: {accuracy_final}")
            # print(f"Final loss: {loss_final}")
        return loss, len(testloader.dataset), {"accuracy": accuracy}


# Start Flower client
fl.client.start_client(
    server_address="127.0.0.1:8080",
    client=FlowerClient().to_client(),
)
