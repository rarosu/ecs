#pragma once

#include <cstdint>

namespace ECS
{
    class Entity
    {
    public:
        int id;
        int64_t flag;
    };
    
    class EntityManager
    {
    public:
        Entity* CreateEntity()
        {
            return nullptr;
        }
        
        void RemoveEntity(Entity* entity)
        {
            
        }
        
        template <typename T>
        T* AddComponent(Entity* entity)
        {
            return nullptr;
        }
        
        template <typename T>
        void RemoveComponent(Entity* entity)
        {
            
        }
    };
}