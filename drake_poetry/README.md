# Python Project with Drake Installed from pip

This installs Drake using [`poetry`](https://python-poetry.org/), the Pyton package manager.

For an introduction to `poetry`, see [Basic usage](https://python-poetry.org/docs/basic-usage/).

## Instructions

First, install the required packages:

```
sudo setup/install_prereqs
```

Additionally, install `poetry` following the [Installation Instructions](https://python-poetry.org/docs/#installation).

Create a new `poetry` project in the current directory, optionally including `-n` to ignore the interactive setup:

```
poetry init [-n]
```

The `pyproject.toml` file that's created contains the project and dependency information. Refer to [Source Installation](https://drake.mit.edu/from_source.html) for the version of Python currently supported by Drake, which can be specified using:

```
requires-python = "3.10"
```

Drake will be added under `dependencies`, which can be done automatically using:

```
poetry add drake
```

To activate the virtual environment installed by `poetry`, use:

```
eval $(poetry env activate)
```

## Examples

To run the particle example tests in this directory, navigate to `src` and call the test file to execute the unit tests:

```
cd src
python particle_test.py
```