#pragma once

#include <set>
#include <bitset>
#include "config.h"
#include "entity.h"

namespace ECS
{
    class SystemManager;

    /**
     * @brief An entity system base class.
     *
     * An entity system will process entities that have a list of components that matches
     * the aspect of the system.
     *
     * To start using a system, it has to be added to a SystemManager that is associated to an EntityManager.
     *
     */
    class EntitySystem
    {
        friend class SystemManager;
    public:
        virtual ~EntitySystem();

        /**
         * @brief Process all entities matching the set aspect.
         *
         */
        void Process();

        /**
         * @brief Process one of the entities in our processing list.
         *
         * This function is called for every entity matching our aspect when Process is called.
         *
         * @param entity The entity to process.
         */
        virtual void ProcessEntity(Entity entity) = 0;

        /**
         * @brief Get the aspect of the system.
         *
         */
        const std::bitset<MAX_COMPONENTS>& GetAspect() const;
    private:
        /**
         * @brief The aspect of the system.
         *
         * An entity is required to have all components specified by the aspect to be processed
         * by a system.
         */
        std::bitset<MAX_COMPONENTS> aspect;

        /**
         * @brief The current set of entities that matches our aspect and should be processed.
         *
         */
        std::set<Entity> entities;
    };
}
