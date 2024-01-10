import uuid

from pydantic import BaseModel, Field
from typing import List, Union


class MLModelParam(BaseModel):
    values: List[Union[int, float]] = Field(title="Parameter values")
    name: str = Field(title="Name of the parameter collection")

    def to_cbor(self):
        pass

class MLModelOperator(BaseModel):
    params: List[int] = Field(title="List of parameter indexes")
    name: str = Field(title="Operator name")

    def to_cbor(self):
        pass

class MLModel(BaseModel):
    identifier: uuid.UUID
    parameters: List[MLModelParam] = Field(title="Parameters associated with this model")
    operators: List[MLModelOperator] = Field(title="Operators associated with this model")

    def to_cbor(self):
        pass


class BaseMLModel(BaseModel):
    identifier: uuid.UUID