# Python Project with Drake Installed from pip

This installs Drake using [`pip`](https://pypi.org/project/pip/), the Pyton package manager.

## Instructions

First, install the required packages:

```
sudo setup/install_prereqs
```

Create a [virtual environment](https://packaging.python.org/en/latest/guides/installing-using-pip-and-virtual-environments/#creating-a-virtual-environment) named `env` and activate it:

```
python -m venv env
source env/bin/activate
```

Then install Drake for Python in the virtual environment:

```
pip install drake
```

Call the following to ensure `pydrake` can be imported:

```
python -c 'import pydrake.all; print(pydrake.__file__)'
```

That's all that is needed to use Drake from Python. See [Installation via Pip](https://drake.mit.edu/pip.html#stable-releases) for more information on installation. For more information on what's available for Drake in Python, see [Using Drake from Python](https://drake.mit.edu/python_bindings.html) and the Python API [pydrake](https://drake.mit.edu/pydrake/index.html).

## Examples

To run the particle example tests in this directory, navigate to `src` and call the test file to execute the unit tests:

```
cd src
python particle_test.py
```