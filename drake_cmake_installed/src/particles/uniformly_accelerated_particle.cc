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

///
/// @brief  A simple 1DOF, constantly accelerated particle example.
///

#include <sys/stat.h>

#include <limits>
#include <memory>
#include <stdexcept>
#include <string>

#include <gflags/gflags.h>

#include <drake/common/drake_copyable.h>
#include <drake/common/eigen_types.h>
#include <drake/common/text_logging_gflags.h>
#include <drake/lcm/drake_lcm.h>
#include <drake/multibody/joints/floating_base_types.h>
#include <drake/multibody/parsers/sdf_parser.h>
#include <drake/multibody/rigid_body_plant/drake_visualizer.h>
#include <drake/multibody/rigid_body_tree.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/framework/context.h>
#include <drake/systems/framework/diagram.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/framework/vector_base.h>
#include <drake/systems/primitives/constant_vector_source.h>

#include "particle.h"
#include "utilities.h"

DEFINE_double(initial_position, 0.0, "Particle initial x position");
DEFINE_double(initial_velocity, 0.0, "Particle initial x velocity");
DEFINE_double(acceleration, 1.0, "Particle constant x acceleration");
DEFINE_double(realtime_rate, 1.0,
              "Rate at which to run the simulation, relative to realtime");
DEFINE_double(simulation_time, std::numeric_limits<double>::infinity(),
              "How long to simulate the particle");

namespace drake_external_examples {
namespace particles {

/// Fixed path to particle SDF model (for visualization purposes only).
static const char* const kParticleSdfPath = "particle.sdf";

/// Check if the specified file exists.
/// @param[in] name of the file
/// @return existence (true) or otherwise (false)
bool file_exists(const std::string& name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

/// A sample diagram for visualizing a 1DOF particle to which a
/// a constant acceleration is applied.
///
/// @tparam T must be a valid Eigen ScalarType.
///
template <typename T>
class UniformlyAcceleratedParticle : public drake::systems::Diagram<T> {
 public:
  DRAKE_NO_COPY_NO_MOVE_NO_ASSIGN(UniformlyAcceleratedParticle)

  /// A constructor that wires up the whole diagram,
  /// taking a constant acceleration to be applied to
  /// the particle and an LCM interface for visualization.
  ///
  /// @param[in] acceleration in m/s^2 units.
  /// @param[in] lcm interface to be used for messaging.
  explicit UniformlyAcceleratedParticle(const T& acceleration,
                                        drake::lcm::DrakeLcmInterface* lcm);

  /// Creates a context using AllocateContext() and sets
  /// state variables according to the initial conditions supplied.
  ///
  /// @param[in] position in m units.
  /// @param[in] velocity in m/s units.
  /// @return a newly created Context.
  std::unique_ptr<drake::systems::Context<T>> CreateContext(
      const T& position, const T& velocity) const;

 private:
  /// RigidBodyTree particle representation
  /// (for visualizations purposes only).
  std::unique_ptr<RigidBodyTree<T>> tree_{std::make_unique<RigidBodyTree<T>>()};
};

template <typename T>
UniformlyAcceleratedParticle<T>::UniformlyAcceleratedParticle(
    const T& acceleration, drake::lcm::DrakeLcmInterface* lcm) {
  // Parse particle sdf into rigid body tree.
  if (!file_exists(kParticleSdfPath)) {
    throw std::runtime_error(std::string("could not find '") +
                             kParticleSdfPath + std::string("'"));
  }
  // using the custom sdf parser from Drake
  drake::parsers::sdf::AddModelInstancesFromSdfFileToWorld(
      kParticleSdfPath, drake::multibody::joints::kRollPitchYaw, tree_.get());
  // Compile tree one more time just to be sure.
  tree_->compile();
  // Building diagram.
  drake::systems::DiagramBuilder<T> builder;
  // Adding constant acceleration source.
  auto constant_acceleration_vector_source =
      builder.template AddSystem<drake::systems::ConstantVectorSource<T>>(
          acceleration);
  // Adding particle.
  auto particle = builder.template AddSystem<Particle<T>>();
  // Adding particle joint.
  drake::MatrixX<T> translating_matrix(6, 1);
  // Only first generalized coordinate gets through.
  translating_matrix.setZero();
  translating_matrix(0, 0) = 1.0;
  auto particle_joint = builder.template AddSystem(
      drake_external_examples::particles::MakeDegenerateEulerJoint(translating_matrix));
  // Adding visualizer client.
  auto visualizer =
      builder.template AddSystem<drake::systems::DrakeVisualizer>(*tree_, lcm);
  // Wiring all blocks together.
  builder.Connect(*constant_acceleration_vector_source, *particle);
  builder.Connect(*particle, *particle_joint);
  builder.Connect(*particle_joint, *visualizer);
  builder.BuildInto(this);
}

template <typename T>
std::unique_ptr<drake::systems::Context<T>>
UniformlyAcceleratedParticle<T>::CreateContext(const T& position,
                                               const T& velocity) const {
  // Allocate context.
  auto context = this->AllocateContext();
  // Set continuous state.
  drake::systems::VectorBase<T>& cstate =
      context->get_mutable_continuous_state_vector();
  cstate.SetAtIndex(0, position);
  cstate.SetAtIndex(1, velocity);
  return context;
}

///
/// Main function for demo.
///
int main(int argc, char* argv[]) {
  gflags::SetUsageMessage(
      "A very simple demonstration, "
      "make sure drake-visualizer is running!");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  drake::logging::HandleSpdlogGflags();
  // Instantiate interface and start receiving.
  auto interface = std::make_unique<drake::lcm::DrakeLcm>();
  interface->StartReceiveThread();
  // Instantiate example system.
  auto system = std::make_unique<UniformlyAcceleratedParticle<double>>(
      FLAGS_acceleration, interface.get());
  // Get context with initial conditions.
  auto context =
      system->CreateContext(FLAGS_initial_position, FLAGS_initial_velocity);
  // Instantiate and configure simulator.
  auto simulator = std::make_unique<drake::systems::Simulator<double>>(
      *system, std::move(context));
  simulator->set_target_realtime_rate(FLAGS_realtime_rate);
  simulator->Initialize();
  // Run simulation.
  simulator->StepTo(FLAGS_simulation_time);
  return 0;
}

}  // namespace particles
}  // namespace drake_external_examples

int main(int argc, char** argv) {
  return drake_external_examples::particles::main(argc, argv);
}
