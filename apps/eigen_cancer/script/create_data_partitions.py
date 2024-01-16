import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.datasets import load_iris, load_breast_cancer, load_wine
from sklearn.preprocessing import MinMaxScaler
from common import create_lda_partitions
from sdv.metadata import SingleTableMetadata
from sdv.single_table import GaussianCopulaSynthesizer, CTGANSynthesizer

import argparse

argparser = argparse.ArgumentParser()
argparser.add_argument("--dataset", type=str, default="iris")
argparser.add_argument("--num_partitions", type=int, default=5)
argparser.add_argument("--concentration", type=float, default=1e-6)
argparser.add_argument("--data_augment", type=bool, default=True)
argparser.add_argument("--niid_partition", type=bool, default=False)
args = argparser.parse_args()

# Load breast cancer dataset
data_name = args.dataset
if data_name == "cancer":
    data = load_breast_cancer()
elif data_name == "iris":
    data = load_iris()
elif data_name == "wine":
    data = load_wine()
features = data.data
labels = data.target
scaler = MinMaxScaler()
normalized_features = scaler.fit_transform(features)

# generate synthetic data to expand the dataset using SDV library
if args.data_augment:
    data_df = pd.DataFrame(normalized_features, columns=data.feature_names)
    data_df["label"] = labels

    # Function to apply Gaussian noise to a DataFrame with labels
    def apply_gaussian_noise(dataframe, noise_factor=0.1):
        # Make a copy of the original DataFrame
        noisy_data = dataframe.copy()
        features = noisy_data.drop(columns=["label"])
        noise = np.random.normal(loc=0, scale=noise_factor, size=features.shape)
        # Apply the noise to the copy
        noisy_data.iloc[:, :-1] += noise
        return noisy_data

    # List of scales for augmentation
    scales = [0.05, 0.1, 0.15, 0.2, 0.25]

    # Generate augmented data
    augmented_data = [
        apply_gaussian_noise(data_df, noise_factor=scale) for scale in scales
    ]

    # Concatenate the list of DataFrames into a single DataFrame
    augmented_df = pd.concat(augmented_data, ignore_index=True)
    synth_features = augmented_df.drop(columns=["label"]).to_numpy()
    synth_labels = augmented_df["label"].to_numpy()


# Perform MinMax scaling on features
# scaler = MinMaxScaler()
# normalized_synth_features = scaler.fit_transform(synth_features)
# normalized_features = scaler.fit_transform(features)
normalized_synth_features = synth_features

# Split the data into train (80%) and validation (20%) sets
features_train, features_val, labels_train, labels_val = train_test_split(
    normalized_synth_features,
    synth_labels,
    test_size=0.2,
    random_state=42,
    shuffle=True,
)
if args.niid_partition:
    # NON-IID partitioning
    data_partitions, d_d = create_lda_partitions(
        dataset=(features_train, labels_train),
        num_partitions=args.num_partitions,
        accept_imbalanced=True,
        concentration=args.concentration,
    )

    for idx, (partition, dis) in enumerate(zip(data_partitions, d_d)):
        # Concatenate features and labels for training and validation sets
        # print(f"Partition {idx} has {dis}")

        train_data = np.column_stack((partition[0], partition[1]))
        # Calculate label counts for the current partition
        unique_labels, label_counts = np.unique(partition[1], return_counts=True)

        # Print label counts for the current partition
        print(f"Label Counts for Partition {idx}:")
        for label, count in zip(unique_labels, label_counts):
            print(f"Label {label}: {count}")
        print("\n")

        # Create Pandas DataFrames
        train_df = pd.DataFrame(
            train_data, columns=np.append(data.feature_names, "label")
        )
        train_df.to_csv(
            f"../data/train_niid_{data_name}_norm_{idx}.csv", header=None, index=False
        )
else:
    # IID partitioning
    # Shuffle the data
    indices = np.arange(len(features_train))
    np.random.shuffle(indices)

    shuffled_features_train = features_train[indices]
    shuffled_labels_train = labels_train[indices]

    # Number of partitions
    n_partitions = args.num_partitions

    # Split shuffled features_train and shuffled_labels_train into n_partitions
    features_train_partitions = np.array_split(shuffled_features_train, n_partitions)
    labels_train_partitions = np.array_split(shuffled_labels_train, n_partitions)
    for idx, (feat, lbl) in enumerate(
        zip(features_train_partitions, labels_train_partitions)
    ):
        train_data = np.column_stack((feat, lbl))
        # Calculate label counts for the current partition
        unique_labels, label_counts = np.unique(lbl, return_counts=True)

        # Print label counts for the current partition
        print(f"Label Counts for Partition {idx}:")
        for label, count in zip(unique_labels, label_counts):
            print(f"Label {label}: {count}")
        print("\n")

        # Create Pandas DataFrames
        train_df = pd.DataFrame(
            train_data, columns=np.append(data.feature_names, "label")
        )
        train_df.to_csv(
            f"../data/train_iid_{data_name}_norm_{idx}.csv", header=None, index=False
        )

# Save to CSV files without headers
val_data = np.column_stack((features_val, labels_val))
val_df = pd.DataFrame(val_data, columns=np.append(data.feature_names, "label"))
val_df.to_csv(f"../data/val_{data_name}_norm.csv", header=None, index=False)

test_data = np.column_stack((normalized_features, labels))
test_df = pd.DataFrame(test_data, columns=np.append(data.feature_names, "label"))
test_df.to_csv(f"../data/test_{data_name}_norm.csv", header=None, index=False)
