#pragma once

#include "config.h"
#include "entity.h"

namespace ECS
{
    /**
     * @brief An entity system base class.
     *
     * An entity system will process entities that have a list of components that matches
     * the aspect of the system.
     *
     */
    class EntitySystem
    {
    public:
        virtual ~EntitySystem();

        void Process();
        virtual void ProcessEntity(Entity entity) = 0;
    private:
        std::bitset<MAX_COMPONENTS> aspect;
        std::vector<Entity> entities;
    };
}
