# Python Project with Drake Installed from Poetry

This installs Drake using [`poetry`](https://python-poetry.org/), the Python package manager.

For an introduction to `poetry`, see [Basic usage](https://python-poetry.org/docs/basic-usage/).

## Instructions

First, install the required packages. This includes an installation of Poetry using `pipx`;
see the [Installation Instructions](https://python-poetry.org/docs/#installation) for
more information.

```
setup/install_prereqs
```

Call `poetry init` to create project in the current directory, including the following arguments:

* `--python [version]` to introduce a requirement on the version(s) 
of Python that will be supported by this `poetry` project. 
Refer to [Source Installation](https://drake.mit.edu/from_source.html) 
for the version of Python currently supported by Drake 
on each operating system. Refer to 
[Version Constraints](https://python-poetry.org/docs/dependency-specification/#version-constraints)
for guidelines on how to specify Python version requirements in `poetry`.
* (Optional) `-n` to ignore the interactive setup

For example, to setup an environment on Ubuntu 22.04, call:

```
poetry init -n --python ~3.10
```

The `pyproject.toml` file that's created contains the project and dependency information. 

From there, ensure `poetry` is using the correct version of Python, as directed above:

```
poetry env use [3.10/3.12]
```

Drake can be installed using:

```
poetry add drake
```

After a successfull installation, Drake should be seen under `dependencies` in the `pyproject.toml` file.

To activate the virtual environment installed by `poetry`, use:

```
eval $(poetry env activate)
```

## Examples

To run the particle example tests in this directory, navigate to `src` and call the test file to execute the unit tests:

```
cd src
python3 particle_test.py
```