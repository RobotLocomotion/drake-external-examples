include(${drake_DIR}/DrakePythonInfo.cmake)

find_package(pybind11 CONFIG REQUIRED)
include(pybind11Tools)

pybind11_add_module(
    example_module
    MODULE
    example_module.cc)
target_link_libraries(example_module PUBLIC drake::drake)

add_test(
    NAME test_example_module
    COMMAND
        "${PYTHON_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/test_example_module.py")
set_tests_properties(
    test_example_module
    PROPERTIES
        # Using example from `pybind11/tests/test_cmake_build/installed_target`.
        ENVIRONMENT "PYTHONPATH=$<TARGET_FILE_DIR:example_module>:$ENV{PYTHONPATH}")
