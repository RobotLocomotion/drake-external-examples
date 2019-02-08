/*****************************************************************************
 * Copyright (c) 2017, Massachusetts Institute of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

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
///     - linear acceleration (input index 0), in @f$ m/s^2 @f$ units.
/// - States/Outputs:
///     - linear position (state/output index 0), in @f$ m @f$ units.
///     - linear velocity (state/output index 1), in @f$ m/s @f$ units.
///
/// @tparam T must be a valid Eigen ScalarType.
///
/// @note
/// Instantiated templates for the following scalar types
/// @p T are provided:
/// - double
///
template <typename T>
class Particle : public drake::systems::LeafSystem<T> {
 public:
  DRAKE_NO_COPY_NO_MOVE_NO_ASSIGN(Particle)

  /// A constructor that initializes the system.
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
