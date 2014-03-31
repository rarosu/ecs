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

    // Make sure the number of entities is correct.
    ASSERT_EQ(ENTITY_COUNT, entityManager.entities.size());

    // Make sure the component table has been extended properly.
    for (int i = 0; i < ECS::MAX_COMPONENTS; ++i)
    {
        ASSERT_EQ(ENTITY_COUNT, entityManager.components[i].size());
    }
}

TEST_F(EntityManagerTest, EntityRemoval)
{
    ECS::Entity e = entityManager.CreateEntity();
    ASSERT_FALSE(entityManager.IsRemoved(e));
    ASSERT_FALSE(entityManager.IsDestroyed(e));

    entityManager.RemoveEntity(e);
    ASSERT_TRUE(entityManager.IsRemoved(e));
    ASSERT_FALSE(entityManager.IsDestroyed(e));

    entityManager.DestroyRemoved();
    ASSERT_TRUE(entityManager.IsRemoved(e));
    ASSERT_TRUE(entityManager.IsDestroyed(e));
}

TEST_F(EntityManagerTest, EntityRemovalCleanup)
{
    const int ENTITY_COUNT = 10;
    for (int i = 0; i < ENTITY_COUNT; ++i)
    {
        ECS::Entity e = entityManager.CreateEntity();
        entityManager.AddComponent<Component1>(e);
        entityManager.AddComponent<Component2>(e);
        entityManager.RemoveEntity(e);
    }

    entityManager.DestroyRemoved();

    // Make sure all components are destroyed and nulled.
    for (int i = 0; i < ECS::MAX_COMPONENTS; ++i)
    {
        for (size_t k = 0; k < entityManager.components[i].size(); ++k)
        {
            ASSERT_EQ(nullptr, entityManager.components[i][k]);
        }
    }

    // Make sure the translator is emptied.
    ASSERT_EQ(0, entityManager.translator.size());

    // Make sure all flags are reset.
    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        ASSERT_TRUE(entityManager.entities[i].flags.none());
    }

    // Make sure all IDs have been recycled.
    ASSERT_EQ(ENTITY_COUNT, entityManager.recycledIds.size());
    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        ASSERT_EQ(i, entityManager.recycledIds[i]);
    }
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

TEST_F(EntityManagerTest, RemoveComponents)
{
    ECS::Entity e = entityManager.CreateEntity();

    Component1* c1 = entityManager.AddComponent<Component1>(e);
    ASSERT_EQ(c1, entityManager.GetComponent<Component1>(e));
    ASSERT_TRUE(entityManager.HasComponent<Component1>(e));
    ASSERT_FALSE(entityManager.IsComponentRemoved<Component1>(e));

    entityManager.RemoveComponent<Component1>(e);
    ASSERT_EQ(c1, entityManager.GetComponent<Component1>(e));
    ASSERT_TRUE(entityManager.HasComponent<Component1>(e));
    ASSERT_TRUE(entityManager.IsComponentRemoved<Component1>(e));

    entityManager.DestroyRemoved();
    ASSERT_EQ(nullptr, entityManager.GetComponent<Component1>(e));
    ASSERT_FALSE(entityManager.HasComponent<Component1>(e));
    ASSERT_FALSE(entityManager.IsComponentRemoved<Component1>(e));
}



/**
 * @brief An implementation of an entity observer, used for testing.
 *
 */
class EntityObserverImpl : public ECS::EntityObserver
{
public:
    struct EntityComponentPair
    {
        ECS::Entity entity;
        ECS::ComponentType componentType;

        EntityComponentPair(ECS::Entity entity, ECS::ComponentType componentType)
        {
            this->entity = entity;
            this->componentType = componentType;
        }

        bool operator<(const EntityComponentPair& rhs)
        {
            return this->entity < rhs.entity &&
                   this->componentType < rhs.componentType;
        }
    };

    void EntityCreated(ECS::Entity entity)
    {
        entitiesCreated.push_back(entity);
    }

    void EntityRemoved(ECS::Entity entity)
    {
        entitiesRemoved.push_back(entity);
    }

    void ComponentAdded(ECS::Entity entity, ECS::ComponentType componentType)
    {
        componentsAdded.push_back(EntityComponentPair(entity, componentType));
    }

    void ComponentRemoved(ECS::Entity entity, ECS::ComponentType componentType)
    {
        componentsRemoved.push_back(EntityComponentPair(entity, componentType));
    }

    std::vector<ECS::Entity> entitiesCreated;
    std::vector<ECS::Entity> entitiesRemoved;
    std::vector<EntityComponentPair> componentsAdded;
    std::vector<EntityComponentPair> componentsRemoved;
};

TEST_F(EntityManagerTest, ObserverEvents)
{
    EntityObserverImpl observer;
    entityManager.AddEntityObserver(&observer);

    // Create entities
    const int ENTITY_COUNT = 10;
    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        entityManager.CreateEntity();
        ASSERT_EQ(i, observer.entitiesCreated.back());
    }

    // Add components
    entityManager.AddComponent<Component1>(1);
    ASSERT_EQ(1, observer.componentsAdded.back().entity);
    ASSERT_EQ(Component1::ID, observer.componentsAdded.back().componentType);

    // Remove components
    entityManager.RemoveComponent<Component1>(1);
    ASSERT_EQ(1, observer.componentsRemoved.back().entity);
    ASSERT_EQ(Component1::ID, observer.componentsRemoved.back().componentType);

    // Remove entities
    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        entityManager.RemoveEntity(i);
        ASSERT_EQ(i, observer.entitiesRemoved.back());
    }
}

TEST_F(EntityManagerTest, GetActiveEntities)
{
    const std::set<ECS::Entity>& entities = entityManager.GetActiveEntities();

    // Create entities
    const int ENTITY_COUNT = 10;
    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        entityManager.CreateEntity();
    }

    // Make sure it has all the created entities.
    ASSERT_EQ(ENTITY_COUNT, entities.size());
    for (size_t i = 0; i < ENTITY_COUNT; ++i)
    {
        ASSERT_TRUE(entities.find(i) != entities.end());
    }

    // Remove all odd entities.
    for (size_t i = 1; i < ENTITY_COUNT; i += 2)
    {
        entityManager.RemoveEntity(i);
    }

    // Make sure it doesn't have the removed entities and that it still
    // has the existing entities.
    for (size_t i = 1; i < ENTITY_COUNT; i += 2)
    {
        ASSERT_TRUE(entities.find(i) == entities.end());
    }

    for (size_t i = 0; i < ENTITY_COUNT; i += 2)
    {
        ASSERT_TRUE(entities.find(i) != entities.end());
    }
}


