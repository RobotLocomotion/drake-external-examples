// SPDX-License-Identifier: MIT-0

#pragma once

#include <drake/common/drake_copyable.h>
#include <drake/systems/framework/basic_vector.h>
#include <drake/systems/framework/context.h>
#include <drake/systems/framework/continuous_state.h>
#include <drake/systems/framework/leaf_system.h>

namespace drake_external_examples {
namespace particles {

/// A linear 1DOF particle system.
///
/// With very simple dynamics @f$ \ddot x = a @f$, this system can be
/// described in terms of its:
///
/// - Inputs:
///   - linear acceleration (input index 0), in @f$ m/s^2 @f$ units.
/// - States/Outputs:
///   - linear position (state/output index 0), in @f$ m @f$ units.
///   - linear velocity (state/output index 1), in @f$ m/s @f$ units.
///
/// @tparam T must be a valid Eigen ScalarType.
///
/// @note
/// Instantiated templates for the following scalar types
/// @p T are provided:
///
/// - double
///
template <typename T>
class Particle final : public drake::systems::LeafSystem<T> {
 public:
  DRAKE_NO_COPY_NO_MOVE_NO_ASSIGN(Particle);

  Particle();

 protected:
  void CopyStateOut(const drake::systems::Context<T>& context,
                    drake::systems::BasicVector<T>* output) const;

  void DoCalcTimeDerivatives(
      const drake::systems::Context<T>& context,
      drake::systems::ContinuousState<T>* derivatives) const override;
};

}  // namespace particles
}  // namespace drake_external_examples
