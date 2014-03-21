#pragma once

#ifndef ECS_COMPONENT_COUNT
    #define ECS_COMPONENT_COUNT 32
#endif

#include "entity.h"
#include "component.h"

namespace ECS
{
    /**
     * @brief Manages all entities and components in the world.
     *
     */
    class EntityManager
    {
    public:
        /**
         * @brief Creates an entity without components.
         *
         * @return ECS::Entity*
         */
        Entity* CreateEntity()
        {
            return nullptr;
        }

        /**
         * @brief Marks an entity and its components for removal and removes it from all systems.
         *
         * Note that this function does not actually delete the entity and its components.
         *
         */
        void RemoveEntity(Entity* entity)
        {

        }

        /**
         * @brief Create a component and add it to the entity.
         *
         * Template type T is the concrete type of the component.
         *
         */
        template <typename T>
        T* AddComponent(Entity* entity)
        {
            return nullptr;
        }

        /**
         * @brief Mark a component for removal and remove its flag from the entity.
         *
         * Template type T is the concrete type of the component. This function will also
         * remove the entity from all relevant systems.
         *
         */
        template <typename T>
        void RemoveComponent(Entity* entity)
        {

        }
    };



    /**
     * @brief Processes all entities containing a certain subset of components.
     *
     */
    class EntitySystem
    {
    public:
        virtual void Process() = 0;
    };

    class EntitySystemManager
    {
    public:

    };
}
