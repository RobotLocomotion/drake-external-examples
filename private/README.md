# Private files

These files are for use by the Drake maintainers, and should not be
copied into your own project(s).

* `drake_cmake_external_static/`: A pared-down copy of the drake_cmake_external
example in order to provide CI coverage of installing and linking libdrake
statically.
* `test/`: General testing utilities.
* `upgrade_cmake_externals.py`: A script to automatically upgrade the upstream
dependencies used by the drake_cmake_external example.
