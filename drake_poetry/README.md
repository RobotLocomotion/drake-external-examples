# Python Project with Drake Installed from Poetry

This installs Drake using [`poetry`](https://python-poetry.org/),
the Python package manager.

For an introduction to `poetry`,
see [Basic usage](https://python-poetry.org/docs/basic-usage/).

# Instructions

The `pyproject.toml` file found in this directory contains all
that is needed to start a Poetry project using the most recent
version of Drake.

Run the installation script to perform the following:

* install the required system packages for Drake
* install Poetry using `pipx`; see the
[Installation Instructions](https://python-poetry.org/docs/#installation)
for more information
* install the most recent version of Drake using `poetry install`

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
