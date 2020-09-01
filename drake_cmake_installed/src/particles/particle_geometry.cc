/*****************************************************************************
 * Copyright (c) 2020, Massachusetts Institute of Technology.
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

#include "particle_geometry.h"

#include <memory>
#include <utility>

#include <drake/geometry/geometry_frame.h>
#include <drake/geometry/geometry_ids.h>
#include <drake/geometry/geometry_instance.h>
#include <drake/geometry/geometry_roles.h>
#include <drake/math/rigid_transform.h>

namespace drake_external_examples {
namespace particles {

const ParticleGeometry* ParticleGeometry::AddToBuilder(
    drake::systems::DiagramBuilder<double>* builder,
    const drake::systems::OutputPort<double>& particle_state_port,
    drake::geometry::SceneGraph<double>* scene_graph) {
  DRAKE_THROW_UNLESS(builder != nullptr);
  DRAKE_THROW_UNLESS(scene_graph != nullptr);

  auto particle_geometry = builder->AddSystem(
      std::unique_ptr<ParticleGeometry>(
          new ParticleGeometry(scene_graph)));
  builder->Connect(
      particle_state_port,
      particle_geometry->get_input_port(0));
  builder->Connect(
      particle_geometry->get_output_port(0),
      scene_graph->get_source_pose_port(particle_geometry->source_id_));

  return particle_geometry;
}

ParticleGeometry::ParticleGeometry(drake::geometry::SceneGraph<double>* scene_graph) {
  DRAKE_THROW_UNLESS(scene_graph != nullptr);
  source_id_ = scene_graph->RegisterSource();
  frame_id_ = scene_graph->RegisterFrame(
      source_id_, drake::geometry::GeometryFrame("particle"));

  this->DeclareInputPort("state", drake::systems::kVectorValued, 2);
  this->DeclareAbstractOutputPort(
      "geometry_pose", &ParticleGeometry::OutputGeometryPose);

  const drake::geometry::GeometryId id = scene_graph->RegisterGeometry(
      source_id_, frame_id_,
      std::make_unique<drake::geometry::GeometryInstance>(
          drake::math::RigidTransformd::Identity(),
          std::make_unique<drake::geometry::Sphere>(0.1), "sphere_visual"));
  scene_graph->AssignRole(
      source_id_, id, drake::geometry::MakePhongIllustrationProperties(
          Eigen::Vector4d(1, 0, 0, 1)));
}

void ParticleGeometry::OutputGeometryPose(
    const drake::systems::Context<double>& context,
    drake::geometry::FramePoseVector<double>* poses) const {
  DRAKE_DEMAND(frame_id_.is_valid());

  const auto& input = get_input_port(0).Eval(context);
  const double q = input(0);
  const drake::math::RigidTransformd pose(Eigen::Vector3d{q, 0, 0});

  *poses = {{frame_id_, pose}};
}

}  // namespace particles
}  // namespace drake_external_examples
