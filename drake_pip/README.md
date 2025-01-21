# Python Project with Drake Installed from Pip

This installs Drake using [`pip`](https://pypi.org/project/pip/),
the Python package manager.

## Instructions

First, install the required Ubuntu packages (this step is not needed on Mac):

```
setup/install_prereqs
```

Create a [virtual environment](https://packaging.python.org/en/latest/guides/installing-using-pip-and-virtual-environments/#creating-a-virtual-environment)
named `env` and activate it:

```
python3 -m venv env
source env/bin/activate
```

*Note:* Depending on the system and which version of Python is installed,
it may be beneficial (or necessary) to instantiate the virtual environment
with a specific command. For example, call `python3.10` on Ubuntu 22.04.
Refer to [Source Installation](https://drake.mit.edu/from_source.html)
for the version of Python currently supported by Drake on each
operating system.

Then install Drake for Python in the virtual environment:

```
pip install drake
```

Call the following to ensure `pydrake` can be imported:

```
python3 -c 'import pydrake.all; print(pydrake.__file__)'
```

That's all that is needed to use Drake from Python.
See [Installation via Pip](https://drake.mit.edu/pip.html#stable-releases)
for more information on installation.
For more information on what's available for Drake in Python,
see [Using Drake from Python](https://drake.mit.edu/python_bindings.html)
and the Python API [pydrake](https://drake.mit.edu/pydrake/index.html).

## Examples

To run the particle example tests in this directory, navigate to `src` and call the test file to execute the unit tests:

```
cd src
python3 particle_test.py
```