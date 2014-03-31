#pragma once

#include <vector>
#include "system.h"
#include "entityobserver.h"

namespace ECS
{
    class EntityManager;

    /**
     * @brief Management class for entity systems.
     *
     * This manager will keep track of all existing entity systems and update
     * their processing list.
     *
     */
    class SystemManager : private EntityObserver
    {
        friend class EntityManager;
    public:
        /**
         * @brief Create a system manager. Requires access to the entity manager.
         *
         * @param entityManager The entity manager this system manager should be associated with.
         */
        SystemManager(EntityManager* entityManager);

        /**
         * @brief Destructor - will delete all systems.
         *
         */
        ~SystemManager();

        /**
         * @brief Registers a system within the manager.
         *
         * This will transfer control of the system from the user to the manager. The manager will delete it later.
         *
         * @param system A heap-allocated entity system.
         */
        void RegisterSystem(EntitySystem* system);
    private:
        /**
         * @brief The entity manager this system manager is associated with.
         *
         */
        EntityManager* entityManager;

        /**
         * @brief The list of systems that should be managed. These are assumed to be heap-allocated and not null.
         *
         */
        std::vector<EntitySystem*> systems;

        /**
         * @brief An entity has been created. Do nothing.
         *
         * @param entity The UUID of the new entity.
         */
        void EntityCreated(ECS::Entity entity);

        /**
         * @brief An entity has been removed. Remove from all systems who process that entity.
         *
         * Note that this is called when an entity is removed, not when it is destroyed.
         *
         * @param entity The UUID of the removed entity.
         */
        void EntityRemoved(ECS::Entity entity);

        /**
         * @brief A component has been added to an entity. Rematch entity against all system aspects.
         *
         * @param entity The UUID of the target entity.
         * @param componentType The ID of the component type added.
         */
        void ComponentAdded(ECS::Entity entity, ECS::ComponentType componentType);

        /**
         * @brief A component has been removed from an entity. Rematch entity against all system aspects.
         *
         * Note that this is called when a component is removed, not when it is destroyed. Also note
         * that this function is not called when an entity is removed, even though all components are removed
         * then.
         *
         * @param entity The UUID of the target entity.
         * @param componentType The ID of the component type added.
         */
        void ComponentRemoved(ECS::Entity entity, ECS::ComponentType componentType);

        /**
         * @brief This will add/remove the entity from appropriate systems.
         *
         * It will match the components on the given entity against the aspect of all registered systems. If it
         * matches the aspect, the entity is added, if not, it is removed.
         *
         */
        void RematchEntityForAllSystems(Entity entity);

        /**
         * @brief This will add or remove the entity to/from the given system.
         *
         * @param entity The entity to reconsider
         * @param system The system which we will compare against
         */
        void RematchEntityForSystem(Entity entity, EntitySystem* system);
    };
}
