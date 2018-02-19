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
 * Simple PCL Example, sitting alongside a Drake camera simulation.
 *
 * This is meant to test building / linking / executing with PCL.
 * This example just builds a random point cloud and downsamples it, printing
 * the size before and after.
 */

#include <iostream>
#include <random>
#include <vector>

#include <pcl/filters/voxel_grid.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

namespace shambhala {

int main() {
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

  std::cout << "PointCloud after filtering: " << cloud_filtered->size()
            << std::endl;
  return 0;
}

}  // namespace shambhala

int main() { return shambhala::main(); }
