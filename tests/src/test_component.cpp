#include <gtest/gtest.h>
#include "../include/ecs_include.h"
#include "../include/components.h"

TEST(Component, TypeIDIncreases)
{
    ASSERT_EQ(Component1::ID, 0);
    ASSERT_EQ(Component2::ID, 1);
}
