import uuid

from pydantic import BaseModel, Field
from typing import List, Union
import cbor2 as cbor
import struct


def _encode_to_buf(arr, t="f"):
    return struct.pack(f"{len(arr)}{t}", *arr)


def to_cbor(arr, t="f"):
    bstr = _encode_to_buf(arr, t)
    if t == "e":
        tag = 84
    elif t == "f":
        tag = 85
    elif t == "d":
        tag = 86
    else:
        raise IndexError
    return cbor.CBORTag(tag, bstr)


class MLTrainingParam(BaseModel):
    accuracy: float
    loss: float

    def to_cbor(self):
        return [(self.accuracy * 1000), int(self.loss * 1000)]

    @classmethod
    def from_cbor(cls, data):
        return cls(accuracy=data[0] / 1000.0, loss=data[1] / 1000.0)


class MLModelParam(BaseModel):
    values: List[Union[int, float]] = Field(title="Parameter values")
    name: str = Field(title="Name of the parameter collection")

    def as_cbor(self):
        return [self.name, to_cbor(self.values)]

    def to_cbor(self):
        pass

    @classmethod
    def from_cbor(cls, data):
        return cls(name=data[0], values=data[1])


class MLModelOperator(BaseModel):
    params: List[int] = Field(title="List of parameter indexes")
    name: str = Field(title="Operator name")

    def as_cbor(self):
        return [self.name, self.params]

    def to_cbor(self):
        return cbor.dumps(self.as_cbor(), canonical=True)

    @classmethod
    def from_cbor(cls, data):
        return cls(name=data[0], params=data[1])


class MLModel(BaseModel):
    identifier: uuid.UUID
    parameters: List[MLModelParam] = Field(
        title="Parameters associated with this model"
    )
    operators: List[MLModelOperator] = Field(
        title="Operators associated with this model"
    )

    def as_cbor(self):
        return [
            self.identifier,
            [param.as_cbor() for param in self.parameters],
            [operator.as_cbor() for operator in self.operators],
        ]

    def to_cbor(self):
        return cbor.dumps(self.as_cbor(), canonical=True)

class MLTrainingModel(BaseModel):
    identifier: uuid.UUID
    parameters: List[MLModelParam] = Field(
        title="Parameters associated with this model"
    )
    operators: List[MLModelOperator] = Field(
        title="Operators associated with this model"
    )
    training: MLTrainingParam = Field(
        title="Training data from this model"
    )

    @classmethod
    def from_cbor(cls, data):
        id = data[0]
        param_data = data[1]
        operator_data = data[2]
        training_data = data[3]
        return cls(
            identifier=id,
            parameters=[MLModelParam.from_cbor(params) for params in param_data],
            operators=[MLModelOperator.from_cbor(operators) for operators in operator_data],
            training=MLTrainingParam.from_cbor(training_data),
        )


class BaseMLModel(BaseModel):
    identifier: uuid.UUID
