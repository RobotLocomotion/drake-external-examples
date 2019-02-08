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

#include "particle.h"  // IWYU pragma: associated

#include <memory>

#include <gtest/gtest.h>

#include <drake/systems/framework/input_port.h>
#include <drake/systems/framework/output_port_value.h>
#include <drake/systems/framework/system.h>
#include <drake/systems/framework/vector_base.h>

namespace drake_external_examples {
namespace particles {
namespace {

///
/// A test fixture class for Particle systems.
///
/// @tparam T must be a valid Eigen ScalarType.
///
template <typename T>
class ParticleTest : public ::testing::Test {
 protected:
  /// Arrange a Particle as the Device Under Test.
  void SetUp() override {
    this->dut_ = std::make_unique<Particle<T>>();
    this->context_ = this->dut_->CreateDefaultContext();
    this->output_ = this->dut_->AllocateOutput(*context_);
    this->derivatives_ = this->dut_->AllocateTimeDerivatives();
  }

  /// System (aka Device Under Test) being tested.
  std::unique_ptr<drake::systems::System<T>> dut_;
  /// Context for the given @p dut_.
  std::unique_ptr<drake::systems::Context<T>> context_;
  /// Outputs of the given @p dut_.
  std::unique_ptr<drake::systems::SystemOutput<T>> output_;
  /// Derivatives of the given @p dut_.
  std::unique_ptr<drake::systems::ContinuousState<T>> derivatives_;
};

TYPED_TEST_CASE_P(ParticleTest);

/// Makes sure a Particle output is consistent with its
/// state (position and velocity).
TYPED_TEST_P(ParticleTest, OutputTest) {
  // Initialize state.
  drake::systems::VectorBase<TypeParam>& continuous_state_vector =
      this->context_->get_mutable_continuous_state_vector();
  continuous_state_vector.SetAtIndex(
      0, static_cast<TypeParam>(10.0));  // x0 = 10 m.
  continuous_state_vector.SetAtIndex(
      1, static_cast<TypeParam>(1.0));  // x1 = 1 m/s.
  // Compute outputs.
  this->dut_->CalcOutput(*this->context_, this->output_.get());
  drake::systems::BasicVector<TypeParam>* output_vector =
      this->output_->GetMutableVectorData(0);
  // Check results.
  EXPECT_EQ(output_vector->GetAtIndex(0),
            static_cast<TypeParam>(10.0));  // y0 == x0
  EXPECT_EQ(output_vector->GetAtIndex(1),
            static_cast<TypeParam>(1.0));  // y1 == x1
}

/// Makes sure a Particle system state derivatives are
/// consistent with its state and input (velocity and acceleration).
TYPED_TEST_P(ParticleTest, DerivativesTest) {
  // Set input.
  const drake::systems::InputPort<TypeParam>& input_port =
      this->dut_->get_input_port(0);
  auto input = std::make_unique<drake::systems::BasicVector<TypeParam>>(
      input_port.size());
  input->SetZero();
  input->SetAtIndex(0, static_cast<TypeParam>(1.0));  // u0 = 1 m/s^2
  this->context_->FixInputPort(0, std::move(input));
  // Set state.
  drake::systems::VectorBase<TypeParam>& continuous_state_vector =
      this->context_->get_mutable_continuous_state_vector();
  continuous_state_vector.SetAtIndex(0,
                                     static_cast<TypeParam>(0.0));  // x0 = 0 m
  continuous_state_vector.SetAtIndex(
      1, static_cast<TypeParam>(2.0));  // x1 = 2 m/s
  // Compute derivatives.
  this->dut_->CalcTimeDerivatives(*this->context_, this->derivatives_.get());
  const drake::systems::VectorBase<TypeParam>& derivatives_vector =
      this->derivatives_->get_vector();
  // Check results.
  EXPECT_EQ(derivatives_vector.GetAtIndex(0),
            static_cast<TypeParam>(2.0));  // x0dot == x1
  EXPECT_EQ(derivatives_vector.GetAtIndex(1),
            static_cast<TypeParam>(1.0));  // x1dot == u0
}

REGISTER_TYPED_TEST_CASE_P(ParticleTest, OutputTest, DerivativesTest);

INSTANTIATE_TYPED_TEST_CASE_P(WithDoubles, ParticleTest, double);

}  // namespace
}  // namespace particles
}  // namespace drake_external_examples
