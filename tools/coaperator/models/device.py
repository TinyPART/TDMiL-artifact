from pkg.device import Device
from pydantic import BaseModel, Field
from typing import List
from pkg.site import coapsite

class DevModel(BaseModel):
    ep: str = Field(title="Device endpoint string")
    lifetime: int = Field(title="Registered lifetime of the device")
    path: str = Field(title="Path used for device reregistration")
    links: List[str] = Field(title="Links available on the device")

    @classmethod
    def from_device(cls, dev: Device):
        links = dev.links.to_py()
        path = '/'.join(dev.path)
        return DevModel(ep=dev.registration_parameters['ep'][0], lifetime=dev.lt, path=path, links=[link[0] for link in links])
