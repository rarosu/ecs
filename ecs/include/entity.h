#pragma once

#include <bitset>

namespace ECS
{
    /**
     * @brief An entity in the world.
     *
     * An entity is basically a container for components. This structure only
     * holds a set of flags of what components are associated with this entity.
     */
    class Entity
    {
    public:
        /**
         * A unique ID for the entity. These are reused after entities are removed.
         */
        int id;

        /**
         * A set of flags marking what components the entity contains.
         */
        std::bitset<ECS_COMPONENT_COUNT> components;
    };
}
