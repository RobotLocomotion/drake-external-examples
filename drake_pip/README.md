# Python Project with Drake Installed from Pip

This installs Drake using [`pip`](https://pypi.org/project/pip/),
the Python package manager.

# Instructions

Follow the setup instructions for [Ubuntu](#ubuntu-setup)
or [Mac OS](#mac-setup).

See [Installation via Pip](https://drake.mit.edu/pip.html#stable-releases)
for more information on installation.

## Ubuntu Setup

If on Ubuntu, first install the required packages:

```bash
setup/install_prereqs
```

This script will also run `setup/setup_env`,
which creates the virtual environment for this project and installs Drake.

To start programming, simply activate the environment by calling:

```bash
source env/bin/activate
```

## Mac Setup

If on Mac OS X, simply ensure the correct version of Python
is installed from Homebrew by referring to the
[Supported Configurations](https://drake.mit.edu/installation.html#supported-configurations).

Then, run the following script to create the virtual environment
for this project and install Drake:

```bash
setup/setup_env
```

*Note*: If you have multiple versions of Python installed,
you can specify the correct version as an optional argument
to the script. For example, to use `python3.13`, call:

```bash
setup/setup_env --python-version 3.13
```

Refer to the
[Drake installation documentation](https://drake.mit.edu/installation.html)
for the versions of Python officially supported on each OS
when installing via PyPI.

To start programming, simply activate the environment by calling:

```bash
source env/bin/activate
```

# Examples

To run the particle example tests in this directory,
navigate to `src` and call the test file to execute the unit tests:

```bash
cd src
python3 particle_test.py
```

For more information on what's available for Drake in Python,
see [Using Drake from Python](https://drake.mit.edu/python_bindings.html)
and the Python API [pydrake](https://drake.mit.edu/pydrake/index.html).
