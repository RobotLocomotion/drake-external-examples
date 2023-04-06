// SPDX-License-Identifier: MIT-0

#include "drake/common/find_resource.h"

int main()
{
  drake::FindResourceOrThrow("drake/examples/pendulum/Pendulum.urdf");
  return 0;
}
