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

#pragma once

#include <drake/geometry/scene_graph.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/framework/leaf_system.h>

namespace drake_external_examples {
namespace particles {

/// Expresses a Particle system's visualization geometry to a SceneGraph.
///
/// @system{ParticleGeometry,
///    @input_port{state},
///    @output_port{geometry_pose}
/// }
///
/// The visualization shows the particle as a sphere moving along the x axis.
///
/// This class has no public constructor; instead use the AddToBuilder() static
/// method to create and add it to a DiagramBuilder directly.
class ParticleGeometry final : public drake::systems::LeafSystem<double> {
 public:
  DRAKE_NO_COPY_NO_MOVE_NO_ASSIGN(ParticleGeometry);

  /// Creates, adds, and connects a ParticleGeometry system into the given
  /// `builder`.  Both the `particle_state_port.get_system()` and `scene_graph`
  /// systems must have been added to the given `builder` already.
  ///
  /// The particle_state_port is a 2D state vector [q, qdot].
  ///
  /// The `scene_graph` pointer is not retained by the %ParticleGeometry system.
  /// The return value pointer is an alias of the new %ParticleGeometry system
  /// that is owned by the `builder`.
  static const ParticleGeometry* AddToBuilder(
      drake::systems::DiagramBuilder<double>* builder,
      const drake::systems::OutputPort<double>& particle_state_port,
      drake::geometry::SceneGraph<double>* scene_graph);

 private:
  explicit ParticleGeometry(drake::geometry::SceneGraph<double>*);
  void OutputGeometryPose(const drake::systems::Context<double>&,
                          drake::geometry::FramePoseVector<double>*) const;

  // Geometry source identifier for this system to interact with SceneGraph.
  drake::geometry::SourceId source_id_;
  // The identifier for the particle frame.
  drake::geometry::FrameId frame_id_;
};

}  // namespace particles
}  // namespace drake_external_examples

