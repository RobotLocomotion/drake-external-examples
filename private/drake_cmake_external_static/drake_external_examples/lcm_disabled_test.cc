// SPDX-License-Identifier: MIT-0

#include <drake/common/drake_assert.h>
#include <drake/lcm/drake_lcm.h>

int main() {
    DRAKE_DEMAND(drake::lcm::DrakeLcm::available() == false);

    return 0;
}
