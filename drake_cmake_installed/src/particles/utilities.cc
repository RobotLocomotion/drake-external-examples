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

#include "utilities.h"

#include <drake/common/drake_throw.h>

namespace drake_external_examples {
namespace particles {

template <typename T>
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
  return std::make_unique<typename drake::systems::MatrixGain<T>>(
      full_translator);
}

template std::unique_ptr<drake::systems::MatrixGain<double>>
MakeDegenerateEulerJoint<double>(const drake::MatrixX<double>& translator);

}  // namespace particles
}  // namespace drake_external_examples
