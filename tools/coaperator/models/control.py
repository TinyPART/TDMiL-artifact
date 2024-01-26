import uuid

from pydantic import BaseModel, Field
from typing import List, Any


class BaseControl(BaseModel):
    command: str = Field(title="Command to execute")
    args: List[Any] = Field(title="Arguments to the command")


class ControlResponse(BaseModel):
    reference: int = Field(title="Identifier of the submitted command")