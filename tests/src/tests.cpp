#include <gtest/gtest.h>
#include <ecs.h>

struct Component1 : public ECS::Component<Component1>
{
    int value;
};

struct Component2 : public ECS::Component<Component2>
{
    float foo;
    char bar[32];
};

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Component, TypeIDIncreases)
{
    ASSERT_EQ(Component1::ID, 0);
    ASSERT_EQ(Component2::ID, 1);
}

