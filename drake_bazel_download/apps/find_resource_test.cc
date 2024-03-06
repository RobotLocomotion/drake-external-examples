// SPDX-License-Identifier: MIT-0

#include "drake/common/find_resource.h"
#include "drake/common/text_logging.h"

int main()
{
  drake::logging::set_log_level("debug");
  drake::FindResourceOrThrow("drake/examples/pendulum/Pendulum.urdf");
  return 0;
}
