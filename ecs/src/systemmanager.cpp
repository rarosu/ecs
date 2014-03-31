#include "../include/systemmanager.h"
#include "../include/entitymanager.h"

namespace ECS
{
    SystemManager::SystemManager(EntityManager* entityManager)
    {
        this->entityManager = entityManager;
    }

    SystemManager::~SystemManager()
    {
        for (size_t i = 0; i < systems.size(); ++i)
            delete systems[i];
    }

    void SystemManager::RegisterSystem(EntitySystem* system)
    {
        systems.push_back(system);

        const std::set<Entity>& activeEntities = entityManager->GetActiveEntities();
        for (auto entity : activeEntities)
        {
            RematchEntityForSystem(entity, system);
        }
    }

    void SystemManager::EntityCreated(ECS::Entity) {}

    void SystemManager::EntityRemoved(ECS::Entity entity)
    {
        // Remove the entity from all systems.
        for (auto system : systems)
        {
            system->entities.erase(entity);
        }
    }

    void SystemManager::ComponentAdded(ECS::Entity entity, ECS::ComponentType)
    {
        RematchEntityForAllSystems(entity);
    }

    void SystemManager::ComponentRemoved(ECS::Entity entity, ECS::ComponentType)
    {
        RematchEntityForAllSystems(entity);
    }

    void SystemManager::RematchEntityForAllSystems(ECS::Entity entity)
    {
        for (auto system = systems.begin(); system != systems.end(); system++)
        {
            RematchEntityForSystem(entity, *system);
        }
    }

    void SystemManager::RematchEntityForSystem(Entity entity, EntitySystem* system)
    {
        const std::bitset<MAX_COMPONENTS>& entityFlag = entityManager->GetEntityFlag(entity);
        const std::bitset<MAX_COMPONENTS>& systemAspect = system->GetAspect();

        if ((entityFlag & systemAspect) == systemAspect)
        {
            system->entities.insert(entity);
        }
        else
        {
            system->entities.erase(entity);
        }
    }
}
