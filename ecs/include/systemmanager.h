#pragma once

#include "system.h"

namespace ECS
{
    /**
     * @brief Management class for entity systems.
     *
     * This manager will keep track of all existing entity systems and update
     * their processing list.
     *
     */
    class SystemManager
    {
    public:
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
         * @brief Called when the components on an entity has been changed.
         *
         * @param entity The entity whose components has changed.
         */
        void (Entity entity);

        /**
         * @brief Called when an entity should be removed from all systems.
         *
         * @param entity The entity that should be removed.
         */
        void RemoveEntityFromSystems(Entity entity);

        std::vector<EntitySystem*> systems;
    };
}
