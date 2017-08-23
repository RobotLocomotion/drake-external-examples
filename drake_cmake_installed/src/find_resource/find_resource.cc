///
/// @brief  A simple find_resource test for locating installed drake resources.
///

#include <iostream>
#include <stdexcept>

#include <gtest/gtest.h>

#include "drake/common/find_resource.h"

namespace shambhala {
namespace find_resource {
namespace {

GTEST_TEST(FindResource, Available)
{
    auto result = drake::FindResourceOrThrow("iiwa14_primitive_collision.urdf");
    // it will throw if it does not find the resource correctly
    SUCCEED();
}

    /// Makes sure that find_resource throws when given a unavailable resource.
GTEST_TEST(FindResource, NotAvailable)
{
    ASSERT_THROW(
        {drake::FindResourceOrThrow("nobody_home.urdf");},
        std::runtime_error
    );
}

}
}  // namespace find_resource
}  // namespace shambhala

