// SPDX-License-Identifier: MIT-0

/**
 * @file
 * Provides an example of binding a simple Drake C++ system in nanobind, to be
 * used with pydrake.
 */

#include <nanobind/nanobind.h>

#include "drake/bindings/pydrake/common/cpp_template_pybind.h"
#include "drake/bindings/pydrake/common/default_scalars_pybind.h"

#include "simple_adder.h"

namespace py = nanobind;

using drake::pydrake::CommonScalarPack;
using drake::pydrake::DefineTemplateClassWithDefault;
using drake::pydrake::GetPyParam;
using drake::systems::LeafSystem;

namespace drake_external_examples {
namespace {

NB_MODULE(simple_adder, m) {
  m.doc() = "Example module interfacing with pydrake and Drake C++";

  py::module_::import_("pydrake.systems.framework");

  auto bind_common_scalar_types = [m](auto dummy) {
    using T = decltype(dummy);

    DefineTemplateClassWithDefault<SimpleAdder<T>, LeafSystem<T>>(
        m, "SimpleAdder", GetPyParam<T>())
      .def(py::init<double>(), py::arg("add"));
  };
  type_visit(bind_common_scalar_types, CommonScalarPack{});
}

}  // namespace
}  // namespace drake_external_examples
