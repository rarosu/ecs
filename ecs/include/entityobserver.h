#pragma once

#include "entity.h"
#include "component.h"

namespace ECS
{
    /**
     * @brief Can be inherited from to observe additions/removal of entities and components.
     *
     * An inherited instance of this class can be registered with an EntityManager to observe its
     * additions/removals.
     */
    class EntityObserver
    {
    public:
        /**
         * @brief An entity has been created.
         *
         * @param entity The UUID of the new entity.
         */
        virtual void EntityCreated(ECS::Entity entity) = 0;

        /**
         * @brief An entity has been removed.
         *
         * Note that this is called when an entity is removed, not when it is destroyed.
         *
         * @param entity The UUID of the removed entity.
         */
        virtual void EntityRemoved(ECS::Entity entity) = 0;

        /**
         * @brief A component has been added to an entity.
         *
         * @param entity The UUID of the target entity.
         * @param componentType The ID of the component type added.
         */
        virtual void ComponentAdded(ECS::Entity entity, ECS::ComponentType componentType) = 0;

        /**
         * @brief A component has been removed from an entity.
         *
         * Note that this is called when a component is removed, not when it is destroyed. Also note
         * that this function is not called when an entity is removed, even though all components are removed
         * then.
         *
         * @param entity The UUID of the target entity.
         * @param componentType The ID of the component type added.
         */
        virtual void ComponentRemoved(ECS::Entity entity, ECS::ComponentType componentType) = 0;
    };
}
