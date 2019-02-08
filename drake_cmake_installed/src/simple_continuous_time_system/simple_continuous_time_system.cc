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

#include <Eigen/Core>

#include <drake/common/autodiff.h>  // IWYU pragma: keep
#include <drake/common/drake_assert.h>
#include <drake/common/unused.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/framework/context.h>
#include <drake/systems/framework/continuous_state.h>
#include <drake/systems/framework/vector_system.h>

namespace drake_external_examples {
namespace systems {

/**
 * Simple Continuous Time System
 *
 * xdot = -x + x^3
 * y = x
 */
class SimpleContinuousTimeSystem : public drake::systems::VectorSystem<double> {
 public:
  SimpleContinuousTimeSystem()
      : drake::systems::VectorSystem<double>(0,    // Zero inputs.
                                             1) {  // One output.
    this->DeclareContinuousState(1);               // One state variable.
  }

 private:
  // xdot = -x + x^3
  virtual void DoCalcVectorTimeDerivatives(
      const drake::systems::Context<double>& context,
      const Eigen::VectorBlock<const Eigen::VectorXd>& input,
      const Eigen::VectorBlock<const Eigen::VectorXd>& state,
      Eigen::VectorBlock<Eigen::VectorXd>* derivatives) const {
    drake::unused(context, input);
    (*derivatives)(0) = -state(0) + std::pow(state(0), 3.0);
  }

  // y = x
  virtual void DoCalcVectorOutput(
      const drake::systems::Context<double>& context,
      const Eigen::VectorBlock<const Eigen::VectorXd>& input,
      const Eigen::VectorBlock<const Eigen::VectorXd>& state,
      Eigen::VectorBlock<Eigen::VectorXd>* output) const {
    drake::unused(context, input);
    *output = state;
  }
};

}  // namespace systems
}  // namespace drake_external_examples

int main() {
  // Create the simple system.
  drake_external_examples::systems::SimpleContinuousTimeSystem system;

  // Create the simulator.
  drake::systems::Simulator<double> simulator(system);

  // Set the initial conditions x(0).
  drake::systems::ContinuousState<double>& state =
      simulator.get_mutable_context().get_mutable_continuous_state();
  state[0] = 0.9;

  // Simulate for 10 seconds.
  simulator.StepTo(10);

  // Make sure the simulation converges to the stable fixed point at x=0.
  DRAKE_DEMAND(state[0] < 1.0e-4);

  // TODO(russt): Make a plot of the resulting trajectory.

  return 0;
}
