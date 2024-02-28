// SPDX-License-Identifier: MIT-0

// This test program checks that Drake headers that include third-party headers
// can be successfully included when using drake_bazel_installed. It serves as
// a regression test for Drake moreso than an example program for end-users.

#include "drake/common/drake_assert.h"
#include "drake/common/eigen_types.h"
#include "drake/common/text_logging.h"
#include "drake/manipulation/kuka_iiwa/iiwa_status_receiver.h"

int main() {
  // Check eigen-related include paths.
  drake::VectorX<double> empty;
  DRAKE_DEMAND(empty.size() == 0);

  // Check lcm-related include paths.
  const drake::manipulation::kuka_iiwa::IiwaStatusReceiver iiwa;
  DRAKE_DEMAND(iiwa.num_input_ports() > 0);

  // Check fmt- and spdlog-related include paths.
  drake::log()->info("Info check {}", "passed");

  return 0;
}
