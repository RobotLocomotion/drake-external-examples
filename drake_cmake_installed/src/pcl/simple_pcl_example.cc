/*****************************************************************************
 * Copyright (c) 2017, Toyota Research Institute.
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
 * @file apps/simple_pcl_test.cc
 *
 * Simple PCL Example, sitting alongside a Drake camera simulation.
 *
 * This is meant to test building / linking / executing with PCL.
 * This example just builds a random point cloud and downsamples it, printing
 * the size before and after.
 */

#include <iostream>
#include <random>
#include <memory>

#include <drake/common/autodiff.h>
#include <drake/multibody/rigid_body_plant/rigid_body_plant.h>
#include <drake/multibody/rigid_body_tree.h>
#include <drake/multibody/rigid_body_tree_construction.h>
// Test Eigen header presence (fails if Drake's Eigen w/ additional Autodiff
// support is not utilised).
#include <drake/solvers/mathematical_program.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/sensors/rgbd_camera.h>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

void test_pcl() {
  PointCloudT::Ptr cloud(new PointCloudT());
  PointCloudT::Ptr cloud_filtered(new PointCloudT());

  const int num_points = 100000;
  const float radius = 1;

  std::uniform_real_distribution<double> distribution;
  std::default_random_engine generator;
  auto rand_pt = [&](float pt[3]) {
    for (int i = 0; i < 3; ++i) {
      pt[i] = (distribution(generator) * 2 - 1) * radius;
    }
  };

  cloud->resize(num_points);
  for (int i = 0; i < num_points; ++i) {
    auto& pt = cloud->points[i];
    rand_pt(pt.data);
  }

  std::cout << "PointCloud before filtering: " << cloud->size() << std::endl;

  // Create the filtering object.
  pcl::VoxelGrid<PointT> filter;
  filter.setInputCloud(cloud);
  filter.setLeafSize(0.01f, 0.01f, 0.01f);
  filter.filter(*cloud_filtered);

  std::cout << "PointCloud after filtering: "
      << cloud_filtered->size() << std::endl;
}

void test_drake_camera() {
  // Creates a simple discrete RGB-D camera simulation.
  drake::systems::DiagramBuilder<double> builder;

  auto tree = std::make_unique<RigidBodyTree<double>>();
  drake::multibody::AddFlatTerrainToWorld(tree.get());
  auto plant = builder.AddSystem<drake::systems::RigidBodyPlant<double>>(
      move(tree));

  auto rgbd_camera =
      builder.AddSystem<drake::systems::sensors::RgbdCameraDiscrete>(
          std::make_unique<drake::systems::sensors::RgbdCamera>(
              "rgbd_camera", plant->get_rigid_body_tree(),
              Eigen::Vector3d(-1., 0., 1.),
              Eigen::Vector3d(0., M_PI_4, 0.),
              0.5, 5.,
              M_PI_4, false),
      0.03);

  builder.Connect(
      plant->get_output_port(0),
      rgbd_camera->state_input_port());
  auto diagram = builder.Build();
  auto simulator = std::make_unique<drake::systems::Simulator<double>>(
      *diagram);
  simulator->StepTo(0.1);

  std::cout << "Finished camera simulation." << std::endl;
}

int main() {
  test_pcl();
  test_drake_camera();
  return 0;
}
