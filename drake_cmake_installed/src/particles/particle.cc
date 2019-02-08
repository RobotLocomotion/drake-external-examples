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

#include "particle.h"

#include <drake/systems/framework/framework_common.h>
#include <drake/systems/framework/vector_base.h>

namespace drake_external_examples {
namespace particles {

template <typename T>
Particle<T>::Particle() {
  // A 1D input vector for acceleration.
  this->DeclareInputPort(drake::systems::kVectorValued, 1);
  // Adding one generalized position and one generalized velocity.
  this->DeclareContinuousState(1, 1, 0);
  // A 2D output vector for position and velocity.
  this->DeclareVectorOutputPort(drake::systems::BasicVector<T>(2),
                                &Particle::CopyStateOut);
}

template <typename T>
void Particle<T>::CopyStateOut(const drake::systems::Context<T>& context,
                               drake::systems::BasicVector<T>* output) const {
  // Get current state from context.
  const drake::systems::VectorBase<T>& continuous_state_vector =
      context.get_continuous_state_vector();
  // Write system output.
  output->set_value(continuous_state_vector.CopyToVector());
}

template <typename T>
void Particle<T>::DoCalcTimeDerivatives(
    const drake::systems::Context<T>& context,
    drake::systems::ContinuousState<T>* derivatives) const {
  // Get current state from context.
  const drake::systems::VectorBase<T>& continuous_state_vector =
      context.get_continuous_state_vector();
  // Obtain the structure we need to write into.
  drake::systems::VectorBase<T>& derivatives_vector =
      derivatives->get_mutable_vector();
  // Get current input acceleration value.
  const drake::systems::BasicVector<T>* input_vector =
      this->EvalVectorInput(context, 0);
  // Set the derivatives. The first one is
  // velocity and the second one is acceleration.
  derivatives_vector.SetAtIndex(0, continuous_state_vector.GetAtIndex(1));
  derivatives_vector.SetAtIndex(1, input_vector->GetAtIndex(0));
}

template class Particle<double>;

}  // namespace particles
}  // namespace drake_external_examples
