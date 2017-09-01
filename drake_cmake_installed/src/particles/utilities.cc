#include "utilities.h"

namespace shambhala {
namespace particles {

template<typename T>
std::unique_ptr<typename drake::systems::MatrixGain<T>>
MakeDegenerateEulerJoint(const drake::MatrixX<T>& translator) {
  // Get translation matrix dimensions.
  int input_dof = translator.rows();
  int output_dof = translator.cols();
  // Output degrees of freedom must be 6.
  DRAKE_THROW_UNLESS(input_dof == 6);
  // Cannot have less than 1 input degree of freedom!
  DRAKE_THROW_UNLESS(output_dof > 0);
  // Cannot have more than 5 input degrees of freedom!
  DRAKE_THROW_UNLESS(output_dof < 6);
  // Build 12x2N matrix.
  drake::MatrixX<T> full_translator(2 * input_dof, 2 * output_dof);
  full_translator.setZero();
  full_translator.topLeftCorner(input_dof, output_dof) = translator;
  full_translator.bottomRightCorner(input_dof, output_dof) = translator;
  // Return matrix gain representing the joint.
  return std::make_unique<typename drake::systems::MatrixGain<T>>(full_translator);
}

template std::unique_ptr<drake::systems::MatrixGain<double>>
MakeDegenerateEulerJoint<double>(const drake::MatrixX<double>& translator);

}  // namespace particles
}  // namespace shambhala
