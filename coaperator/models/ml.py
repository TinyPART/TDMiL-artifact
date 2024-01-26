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
    return cbor.CBORTag(tag, bstr)


class MLModelParam(BaseModel):
    values: List[Union[int, float]] = Field(title="Parameter values")
    name: str = Field(title="Name of the parameter collection")

    def as_cbor(self):
        return [self.name, to_cbor(self.values)]

    def to_cbor(self):
        pass


class MLModelOperator(BaseModel):
    params: List[int] = Field(title="List of parameter indexes")
    name: str = Field(title="Operator name")

    def as_cbor(self):
        return [self.name, self.params]

    def to_cbor(self):
        return cbor.dumps(self.as_cbor(), canonical=True)


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


class BaseMLModel(BaseModel):
    identifier: uuid.UUID
