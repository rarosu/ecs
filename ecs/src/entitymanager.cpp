#include "../include/entitymanager.h"

namespace ECS
{
    const std::bitset<MAX_COMPONENTS> EntityManager::ZERO_BITSET;

    EntityManager::EntityManager(size_t reservedEntityCount)
    {
        nextUUID = 0;
        nextInternalId = 0;

        entities.reserve(reservedEntityCount);
        for (size_t i = 0; i < MAX_COMPONENTS; ++i)
            components[i].reserve(reservedEntityCount);
    }

    Entity EntityManager::CreateEntity()
    {
        Entity entity = nextUUID++;
        size_t internalId;
        if (recycledIds.empty())
        {
            // Choose a new internal ID.
            internalId = nextInternalId++;
            entities.push_back(Private::InternalEntity());
            for (size_t i = 0; i < MAX_COMPONENTS; ++i)
                components[i].push_back(nullptr);
        }
        else
        {
            // Use a recycled internal ID.
            internalId = recycledIds.back();
            recycledIds.pop_back();
        }

        translator[entity] = internalId;
        return entity;
    }

    void EntityManager::RemoveEntity(Entity entity)
    {
        // TODO: Remove entity from systems

        auto it = translator.find(entity);
        assert(it != translator.end());

        size_t internalId = it->second;
        assert(internalId < entities.size());

        entitiesToDestroy.push_back(internalId);
        entities[internalId].flags.reset();
    }

    bool EntityManager::IsRemoved(Entity entity)
    {
        auto it = translator.find(entity);

        // If the entity has been destroyed and is an entity that we have created, return true.
        if (it == translator.end())
            return true;

        size_t internalId = it->second;
        assert(internalId < entities.size());

        // If the entity is in the destroy list, it has been removed; return true.
        return std::find(entitiesToDestroy.begin(), entitiesToDestroy.end(), internalId) != entitiesToDestroy.end();
    }

    bool EntityManager::IsDestroyed(Entity entity)
    {
        return translator.find(entity) == translator.end();
    }

    const std::bitset<MAX_COMPONENTS>& EntityManager::GetEntityFlag(Entity entity)
    {
        auto it = translator.find(entity);
        if (it == translator.end())
            return ZERO_BITSET;

        size_t internalId = it->second;
        assert(internalId < entities.size());

        return entities[internalId].flags;
    }

    void EntityManager::DestroyRemoved()
    {
        // TODO: Use custom deallocator instead of delete

        // Destroy all removed entities.
        for (size_t internalId : entitiesToDestroy)
        {
            // Destroy all components associated with the entity.
            for (size_t i = 0; i < MAX_COMPONENTS; ++i)
            {
                delete components[i][internalId];
                components[i][internalId] = nullptr;
            }

            // Reset the component flag
            entities[internalId].flags = 0;

            // Recycle the ID
            recycledIds.push_back(internalId);
        }

        // Destroy all removed components.
        for (auto componentKey : componentsToDestroy)
        {
            delete components[componentKey.second][componentKey.first];
            components[componentKey.second][componentKey.first] = nullptr;
        }
    }
}
