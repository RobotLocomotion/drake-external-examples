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

#pragma once

#include <memory>

#include <drake/common/eigen_types.h>
#include <drake/systems/primitives/matrix_gain.h>

namespace drake_external_examples {
namespace particles {

/// Makes an NDOF to 6DOF poses mapping system.
///
/// The given matrix @f$ \mathbf{M}_{\mathrm{6,N}} @f$ is
/// augmented to deal with position and velocity
/// state input @f$ [\mathbf{q} ; \mathbf{v}] @f$ as follows:
///
/// @f[ M^*_{\mathrm{12,2N}} =
///     \begin{bmatrix}
///       \mathbf{M}_{\mathrm{6,N}} & \mathbf{0}_{\mathrm{6,N}}
///    \\ \mathbf{0}_{\mathrm{6,N}} & \mathbf{M}_{\mathrm{6,N}}
///     \end{bmatrix}
/// @f]
///
/// @param[in] translator 6xN matrix to translate inputs to outputs, where
/// 0 < N < 6.
/// @return MatrixGain representing the joint.
/// @throws std::runtime_error whenever @p translator matrix implies
/// M DOF output where M is not 6, or an N DOF input where N is not a
/// positive non-zero integer that is less than 6.
///
/// @tparam T must be a valid Eigen ScalarType.
///
/// @note
/// Instantiated templates for the following scalar types
/// @p T are provided:
/// - double
///
template <typename T>
std::unique_ptr<typename drake::systems::MatrixGain<T>>
MakeDegenerateEulerJoint(const drake::MatrixX<T>& translator);

}  // namespace particles
}  // namespace drake_external_examples
