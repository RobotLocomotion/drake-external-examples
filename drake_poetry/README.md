# Python Project with Drake Installed from Poetry

This installs Drake using [`poetry`](https://python-poetry.org/),
the Python package manager.

For an introduction to `poetry`,
see [Basic usage](https://python-poetry.org/docs/basic-usage/).

# Instructions

The `pyproject.toml` file found in this directory contains all
that is needed to start a Poetry project using the most recent
version of Drake.

First, follow the [Installation Instructions](https://python-poetry.org/docs/#installation)
to install Poetry using your preferred method.

Run the installation script to perform the following:

* install the required system packages for Drake (Ubuntu only)
* set up your Poetry environment and install the most recent version of Drake

```bash
setup/install_prereqs
```

*Note*: If you have multiple versions of Python installed,
you can specify the correct version as an optional argument
to the script. For example, `setup/install_prereqs 3.12`.

# Examples

To run the particle example tests in this directory,
navigate to `src` and call the test file to execute the unit tests:

```bash
cd src
poetry run python particle_test.py
```

For more information on what's available for Drake in Python,
see [Using Drake from Python](https://drake.mit.edu/python_bindings.html)
and the Python API [pydrake](https://drake.mit.edu/pydrake/index.html).
