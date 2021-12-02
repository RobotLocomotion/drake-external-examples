/*****************************************************************************
 * Copyright (c) 2018, Toyota Research Institute
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
 * Provides an example of testing a custom C++ Drake system which will also
 * be bound in Python.
 */

#include <iostream>

#include <drake/common/text_logging.h>
#include <drake/systems/analysis/simulator.h>
#include <drake/systems/framework/diagram_builder.h>
#include <drake/systems/primitives/constant_vector_source.h>
#include <drake/systems/primitives/vector_log_sink.h>

#include "simple_adder.h"

using drake::systems::Simulator;
using drake::systems::DiagramBuilder;
using drake::systems::ConstantVectorSource;
using drake::systems::VectorLogSink;

namespace drake_external_examples {
namespace {

int DoMain() {
  DiagramBuilder<double> builder;
  auto source = builder.AddSystem<ConstantVectorSource<double>>(
      Eigen::VectorXd::Constant(1, 10.));
  auto adder = builder.AddSystem<SimpleAdder<double>>(100.);
  builder.Connect(source->get_output_port(), adder->get_input_port(0));
  auto logger = builder.AddSystem<VectorLogSink<double>>(1);
  builder.Connect(adder->get_output_port(0), logger->get_input_port());
  auto diagram = builder.Build();

  Simulator<double> simulator(*diagram);
  simulator.AdvanceTo(1);

  auto x = logger->FindLog(simulator.get_context()).data();
  Eigen::VectorXd x_expected = Eigen::Vector2d(110., 110.);
  std::cout << "Output values: " << x << std::endl;
  DRAKE_DEMAND(x.isApprox(x_expected.transpose()));

  return 0;
}

}  // namespace
}  // namespace drake_external_examples

int main() {
  return drake_external_examples::DoMain();
}
