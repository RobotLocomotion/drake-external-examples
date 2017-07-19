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
 * * Neither the name of the Massachusetts Institute of Technology nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE MASSACHUSETTS INSTITUTE OF TECHNOLOGY AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE MASSACHUSETTS
 * INSTITUTE OF TECHNOLOGY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

/**
 * @file apps/simple_continuous_time_system.cc
 *
 * Simple Continuous Time System Example
 *
 * This is meant to be a sort of "hello world" example for the drake::system
 * classes. It defines a very simple continuous time system, simulates it from
 * a given initial condition, and plots the result.
 */

#include <cmath>

#include "drake/common/drake_assert.h"
#include "drake/systems/analysis/simulator.h"
#include "drake/systems/framework/basic_vector.h"
#include "drake/systems/framework/context.h"
#include "drake/systems/framework/continuous_state.h"
#include "drake/systems/framework/leaf_system.h"

namespace shambhala {

/**
 * Simple Continuous Time System
 *
 * xdot = -x + x^3
 * y = x
 */
class SimpleContinuousTimeSystem : public drake::systems::LeafSystem<double> {
 public:
  SimpleContinuousTimeSystem() {
    constexpr int kSize = 1;  // The dimension of both output (y) and state (x).
    this->DeclareVectorOutputPort(drake::systems::BasicVector<double>(kSize),
                                  &SimpleContinuousTimeSystem::CopyStateOut);
    this->DeclareContinuousState(kSize);
  }

 private:
  // xdot = -x + x^3
  void DoCalcTimeDerivatives(
      const drake::systems::Context<double>& context,
      drake::systems::ContinuousState<double>* derivatives) const override {
    const double x = context.get_continuous_state_vector().GetAtIndex(0);
    const double xdot = -x + std::pow(x, 3.0);
    derivatives->get_mutable_vector()->SetAtIndex(0, xdot);
  }

  // y = x
  void CopyStateOut(const drake::systems::Context<double>& context,
                    drake::systems::BasicVector<double>* output) const {
    const double x = context.get_continuous_state_vector().GetAtIndex(0);
    output->SetAtIndex(0, x);
  }
};

}  // namespace shambhala

int main() {
  // Create the simple system.
  shambhala::SimpleContinuousTimeSystem system;

  // Create the simulator.
  drake::systems::Simulator<double> simulator(system);

  // Set the initial conditions x(0).
  drake::systems::ContinuousState<double>& xc =
      *simulator.get_mutable_context()->get_mutable_continuous_state();
  xc[0] = 0.9;

  // Simulate for 10 seconds.
  simulator.StepTo(10);

  // Make sure the simulation converges to the stable fixed point at x=0.
  DRAKE_DEMAND(xc[0] < 1.0e-4);

  // TODO(russt): Make a plot of the resulting trajectory.

  return 0;
}
