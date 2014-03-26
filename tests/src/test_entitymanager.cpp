#include <gtest/gtest.h>
#include "../include/ecs_include.h"
#include "../include/components.h"

/**
 * @brief A fixture for testing the entity manager.
 */
class EntityManagerTest : public ::testing::Test
{
public:
    EntityManagerTest();

    ECS::EntityManager entityManager;
};

EntityManagerTest::EntityManagerTest() : entityManager(1024) {}




TEST_F(EntityManagerTest, EntityCreate)
{
    const int ENTITY_COUNT = 10;
    for (int i = 0; i < ENTITY_COUNT; ++i)
    {
        ASSERT_EQ(i, entityManager.CreateEntity());
    }

    ASSERT_EQ(entityManager.entities.size(), ENTITY_COUNT);
}

TEST_F(EntityManagerTest, AddComponents)
{
    ECS::Entity e = entityManager.CreateEntity();
    Component1* c1 = entityManager.AddComponent<Component1>(e);

    ASSERT_NE(c1, nullptr);
    ASSERT_TRUE(entityManager.HasComponent<Component1>(e));
    ASSERT_TRUE(entityManager.GetEntityFlag(e).test(Component1::ID));
    ASSERT_FALSE(entityManager.GetEntityFlag(e).test(Component2::ID));

    ASSERT_EQ(c1, entityManager.GetComponent<Component1>(e));
}

TEST_F(EntityManagerTest, EntityRecycling)
{
    ECS::Entity e = entityManager.CreateEntity();
    entityManager.RemoveEntity(e);

    // Make sure this entity is not recycled (since a cleanup hasn't happened yet).
    e = entityManager.CreateEntity();
    ASSERT_EQ(e, 1);
    ASSERT_EQ(entityManager.translator[e], 1);

    // Clean up removed entities and create a new one (which should be recycled).
    entityManager.DestroyRemoved();
    e = entityManager.CreateEntity();
    ASSERT_EQ(e, 2);
    ASSERT_EQ(entityManager.translator[e], 0);
}
