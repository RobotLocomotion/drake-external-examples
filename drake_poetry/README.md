# Python Project with Drake Installed from pip

This installs Drake using [`poetry`](https://python-poetry.org/), the Pyton package manager.

For an introduction to `poetry`, see [Basic usage](https://python-poetry.org/docs/basic-usage/).

## Instructions

First, create a new `poetry` project:

```
poetry new drake_poetry_demo
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

