#include <pybind11/pybind11.h>

#include <drake/systems/framework/leaf_system.h>

namespace py = pybind11;

using drake::systems::BasicVector;
using drake::systems::Context;
using drake::systems::LeafSystem;
using drake::systems::kVectorValued;

namespace shambhala {
namespace {

/// Adds a constant to an input.
template <typename T>
class SimpleAdder : public LeafSystem<T> {
 public:
  SimpleAdder(T add)
      : add_(add) {
    this->DeclareInputPort(kVectorValued, 1);
    this->DeclareVectorOutputPort(
        BasicVector<T>(1), &SimpleAdder::CalcOutput);
  }

 private:
  void CalcOutput(const Context<T>& context, BasicVector<T>* output) const {
    auto u = this->EvalEigenVectorInput(context, 0);
    auto&& y = output->get_mutable_value();
    y.array() = u.array() + add_;
  }

  const T add_{};
};

PYBIND11_MODULE(example_module, m) {
  m.doc() = "Example module interfacing with pydrake and Drake C++";

  py::module::import("pydrake.systems.framework");

  using T = double;

  py::class_<SimpleAdder<T>, LeafSystem<T>>(m, "SimpleAdder")
      .def(py::init<T>(), py::arg("add"));
}

}  // namespace
}  // namespace shambhala
