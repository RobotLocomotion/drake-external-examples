/*****************************************************************************
 * Copyright (c) 2018, Toyota Research Institute.
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
 *
 * Test a Drake camera simulation that has been linked with PCL.
 */

#include <cmath>
#include <iostream>
#include <memory>
#include <utility>

#include <Eigen/Core>

// Test Eigen header presence (fails if Drake's Eigen w/ additional Autodiff
// support is not utilised).
#include <drake/common/autodiff.h>  // IWYU pragma: keep
#include <drake/common/find_resource.h>
#include <drake/multibody/parsers/sdf_parser.h>
#include <drake/multibody/rigid_body_plant/rigid_body_plant.h>
#include <drake/multibody/rigid_body_tree.h>
#include <drake/multibody/rigid_body_tree_construction.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/sensors/rgbd_camera.h>

namespace shambhala {

int main() {
  // Creates a simple discrete RGB-D camera simulation.
  drake::systems::DiagramBuilder<double> builder;

  // TODO(eric.cousineau): Switch to MultibodyPlant once the corresponding
  // `RgbdCamera` is no longer in `dev`.
  auto tree = std::make_unique<RigidBodyTree<double>>();
  // TODO(eric.cousineau): Once RobotLocomotion/drake#10105 is resolved, switch
  // back to a zero-DOF tree.
  drake::parsers::sdf::AddModelInstancesFromSdfFileToWorld(
      drake::FindResourceOrThrow(
          "drake/examples/double_pendulum/models/double_pendulum.sdf"),
      drake::multibody::joints::FloatingBaseType::kQuaternion,
      tree.get());
  drake::multibody::AddFlatTerrainToWorld(tree.get());
  auto plant = builder.AddSystem<drake::systems::RigidBodyPlant<double>>(
      std::move(tree));
  plant->set_name("plant");

  const double period = 0.03;
  auto rgbd_camera =
      builder.AddSystem<drake::systems::sensors::RgbdCameraDiscrete>(
          std::make_unique<drake::systems::sensors::RgbdCamera>(
              "rgbd_camera", plant->get_rigid_body_tree(),
              Eigen::Vector3d(-1., 0., 1.), Eigen::Vector3d(0., M_PI_4, 0.),
              0.5, 5., M_PI_4, false),
          period);

  builder.Connect(plant->state_output_port(), rgbd_camera->state_input_port());
  auto diagram = builder.Build();
  auto simulator =
      std::make_unique<drake::systems::Simulator<double>>(*diagram);
  simulator->StepTo(0.1);

  // TODO(eric.cousineau): Add in example of converting depth image to PCL
  // point cloud.

  std::cout << "Finished camera simulation." << std::endl;
  return 0;
}

}  // namespace shambhala

int main() { return shambhala::main(); }
