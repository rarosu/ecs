#include "../include/system.h"

namespace ECS
{
    EntitySystem::~EntitySystem() {}

    void EntitySystem::Process()
    {
        // TODO: Figure out a way of looping properly when entities can be added/removed in every ProcessEntity.
        for (auto entity : entities)
        {
            ProcessEntity(entity);
        }
    }

    const std::bitset<MAX_COMPONENTS>& EntitySystem::GetAspect() const
    {
        return aspect;
    }
}
