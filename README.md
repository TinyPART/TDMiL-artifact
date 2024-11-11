# IEEE Access TDMiL Artifact 

This report contains the source code artifact for the IEEE Access'24 paper on
TDMiL. 

To refer to this work, please cite:

```
M. Gulati, K. Zandberg, Z. Huang, G. Wunder, C. Adjih, E. Baccelli. TDMiL: Tiny Distributed Machine Learning for Microcontroller-Based Interconnected Devices. In IEEE Access, Oct. 2024.
```

## Repository Outline

This repository consists of multiple directories with components.

- **RIOT**: The directory containing the RIOT source code with minimal modifications
- **apps**: Demonstration firmware application for the research artifact using RIOT
as RTOS.
- **coaperator**: The CoAPerator orchestrator application written in Python
- **modules**: Custom modules for the RIOT-based firmware applications
- **radvd.d**: Default configuration for a router advertisement daemon

## How to use this artifact:

### Requirements to run the artifacts

#### RIOT firmware applications

To compile the firmware artifacts, an environment able to compile the RIOT
operating system on Linux is required. A Debian-based Linux distribution is
recommended.
Furthermore the following tools are required:

* Essential system development tools (GNU Make GCC, standard C library headers),
  can usually be installed by installing `build-essential` on Debian and
  derivatives.
* git
* GDB in the multiarch variant (alternatively: install for each architecture you target the
  corresponding GDB package)
* unzip or p7zip
* wget or curl
* python3
* pyserial (linux distro package often named python3-serial or py3-serial)

These can be installed on Debian and Ubuntu via

```Console
# apt install gcc-multilib build-essential git llvm clang python3 python3-pip wget curl unzip
```

#### CoAPerator

The CoAPerator orchestrator application written in Python requires at least
`Python 3.11`. The python dependencies can be installed using
[poetry](https://python-poetry.org/) as detailed in the `coaperator` directory.

## Experiments

### Basic experiment

The first experiment launches CoAPerator (the orchestrator) and then a client (TDMiL).
The client can then be instructed via CoAPerator to download a model via CoAP.
For this, please follow the instructions in `apps/FL_ML_client/readme.md`

### Other experiments
Additional experiments can be found in the `apps` directory. Head to these directories for
instructions on how to run them.

