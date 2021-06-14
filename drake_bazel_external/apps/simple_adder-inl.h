/*****************************************************************************
 * Copyright (c) 2018-2021, Toyota Research Institute
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

/**
 * @file
 * Provides an example of creating a simple Drake C++ system that will later be
 * bound.
 *
 * This is the definition portion of the -inl.h pattern:
 * http://drake.mit.edu/cxx_inl.html#cxx-inl-files
 */

#include "simple_adder.h"

namespace drake_external_examples {

using drake::systems::BasicVector;
using drake::systems::Context;
using drake::systems::LeafSystem;
using drake::systems::kVectorValued;

template <typename T>
SimpleAdder<T>::SimpleAdder(T add)
      : add_(add) {
  this->DeclareInputPort("in", kVectorValued, 1);
  this->DeclareVectorOutputPort(
      "out", BasicVector<T>(1), &SimpleAdder::CalcOutput);
}

template <typename T>
void SimpleAdder<T>::CalcOutput(
    const Context<T>& context, BasicVector<T>* output) const {
  auto u = this->EvalEigenVectorInput(context, 0);
  auto&& y = output->get_mutable_value();
  y.array() = u.array() + add_;
}

}  // namespace drake_external_examples
