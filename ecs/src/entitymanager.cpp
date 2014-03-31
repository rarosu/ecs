#include "../include/entitymanager.h"

namespace ECS
{
    const std::bitset<MAX_COMPONENTS> EntityManager::ZERO_BITSET;


    EntityManager::ComponentReference::ComponentReference(size_t internalEntityId, ComponentType componentType)
    {
        this->internalEntityId = internalEntityId;
        this->componentType = componentType;
    }

    bool EntityManager::ComponentReference::operator==(const ComponentReference& rhs) const
    {
        return internalEntityId == rhs.internalEntityId &&
               componentType == rhs.componentType;
    }


    EntityManager::EntityManager(size_t reservedEntityCount)
    {
        nextUUID = 0;
        nextInternalId = 0;

        entities.reserve(reservedEntityCount);
        for (size_t i = 0; i < MAX_COMPONENTS; ++i)
            components[i].reserve(reservedEntityCount);
    }

    EntityManager::~EntityManager()
    {
        // TODO: Use custom deallocator.
        for (size_t i = 0; i < MAX_COMPONENTS; ++i)
        {
            for (size_t k = 0; k < entities.size(); ++k)
            {
                delete components[i][k];
            }
        }
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
        activeEntities.insert(entity);

        // Notify all observers of the created entity.
        for (auto observer : observers)
            observer->EntityCreated(entity);

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
        activeEntities.erase(entity);

        for (auto observer : observers)
            observer->EntityRemoved(entity);
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

    const std::set<Entity>& EntityManager::GetActiveEntities() const
    {
        return activeEntities;
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
        for (Entity entity : entitiesToDestroy)
        {
            size_t internalId = translator.find(entity)->second;

            // Destroy all components associated with the entity.
            for (size_t i = 0; i < MAX_COMPONENTS; ++i)
            {
                delete components[i][internalId];
                components[i][internalId] = nullptr;
            }

            // Reset and recycle
            entities[internalId].flags.reset();
            translator.erase(entity);
            recycledIds.push_back(internalId);
        }

        // Destroy all removed components.
        for (auto componentKey : componentsToDestroy)
        {
            delete components[componentKey.componentType][componentKey.internalEntityId];
            components[componentKey.componentType][componentKey.internalEntityId] = nullptr;
        }
    }

    void EntityManager::AddEntityObserver(EntityObserver* observer)
    {
        observers.insert(observer);
    }

    void EntityManager::RemoveEntityObserver(EntityObserver* observer)
    {
        observers.erase(observer);
    }

    bool EntityManager::IsObserving(EntityObserver* observer)
    {
        return observers.find(observer) != observers.end();
    }
}
