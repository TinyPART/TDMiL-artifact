import logging
import uuid
from typing import Dict, Optional
from models.ml import MLModel


class MLModelStore(object):
    _instance = None

    @classmethod
    def __new__(cls, *args):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
            cls._instance.models: Dict[uuid.UUID: MLModel] = {}
        return cls._instance

    def list(self):
        return self.models.keys()

    def add_model(self, model: MLModel) -> uuid.UUID:
        if model.identifier == None:
            uid = uuid.uuid4()
            model.identifier = uid
        self.models[model.identifier] = model
        return model.identifier

    def get_model(self, uid: uuid.UUID) -> Optional[MLModel]:
        return self.models.get(uid)