#pragma once

#include <bitset>
#include "config.h"

namespace ECS
{
    /**
     * @brief Entity type. A UUID (in this context) to identify an entity.
     *
     * This is the only handle used externally. It is unique so that removed and recycled
     * entities will not be accessed accidentally.
     */
    typedef uint64_t Entity;

    namespace Private
    {
        struct InternalEntity
        {
            /**
             * @brief Defines what components are associated with this entity.
             *
             */
            std::bitset<MAX_COMPONENTS> flags;
        };
    }
}
