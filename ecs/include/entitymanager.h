#pragma once

#include <cassert>
#include <vector>
#include <map>
#include <algorithm>
#include "config.h"
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
         * @brief Constructor. Set default values and reserve memory.
         *
         * @param reservedEntityCount How many entities to reserve memory for to start with.
         */
        EntityManager(size_t reservedEntityCount = RESERVED_ENTITY_COUNT);

        /**
         * @brief Creates an entity without components.
         *
         * @return The created entity.
         */
        Entity CreateEntity();

        /**
         * @brief Marks an entity and its components for removal and removes it from all systems.
         *
         * This function will remove the entity from the processing list of all systems. It will not
         * recycle or remove its components however; this will be done after the current system is finished
         * processing.
         *
         */
        void RemoveEntity(Entity entity);

        /**
         * @brief Check if an entity has been removed or destroyed.
         *
         * @return True if the entity has been destroyed or is in the entitiesToDestroy list.
         */
        bool IsRemoved(Entity entity);

        /**
         * @brief Check if an entity has been destroyed.
         *
         * Note that this function will not return true if the entity is in the entitiesToDestroy list.
         *
         * @return True if the entity has been destroyed, false otherwise.
         */
        bool IsDestroyed(Entity entity);


        /**
         * @brief Get a bitset determining what components this entity has.
         *
         * The component type ID can be used to index into the bitset and see whether
         * that particular component is associated with this entity.
         *
         * Deleted and removed entities always return an empty bitset. Removed components
         * has a zero on its index.
         *
         * @return The component flags associated with this entity.
         */
        const std::bitset<MAX_COMPONENTS>& GetEntityFlag(Entity entity);

        /**
         * @brief Create a component and add it to the entity.
         *
         * Template type T is the concrete type of the component.
         *
         * @return The created component.
         */
        template <typename T>
        T* AddComponent(Entity entity);

        /**
         * @brief Get the component of type T on entity.
         *
         * @return Component or nullptr if no component of type T exists on the entity.
         */
        template <typename T>
        T* GetComponent(Entity entity);

        /**
         * @brief Mark a component for removal and remove its flag from the entity.
         *
         * Template type T is the concrete type of the component. This function will also
         * remove the entity from all relevant systems.
         *
         */
        template <typename T>
        void RemoveComponent(Entity entity);

        /**
         * @brief Check if the entity has a component.
         *
         * Template type T is the concrete type of the component.
         *
         * This function will return true if the component has been removed but not destroyed yet.
         * @see IsComponentRemoved
         *
         * @return True if the entity has the component specified by T (even if removed but not destroyed). False otherwise.
         */
        template <typename T>
        bool HasComponent(Entity entity) const;

        /**
         * @brief Check if the entity has a component that is removed (but not destroyed yet).
         *
         * Removed components are not destroyed until a system has finished processing. This function
         * checks if the entity has a component that has been removed but not destroyed yet.
         *
         * @return True if the component exists on the entity but is in the componentsToDestroy list.
         */
        template <typename T>
        bool IsComponentRemoved(Entity entity) const;

        /**
         * @brief This will destroy all removed entities and components.
         *
         */
        void DestroyRemoved();
    private:
        static const std::bitset<MAX_COMPONENTS> ZERO_BITSET;

        /**
         * @brief Maps an Entity UUID to an internal entity ID.
         *
         */
        std::map<Entity, size_t> translator;

        /**
         * @brief A vector of all the active entities.
         *
         * The indices in this vector will be recycled whenever an entity is
         * removed.
         */
        std::vector<Private::InternalEntity> entities;

        /**
         * @brief Maps the tuple (component type, internal entity id) to a component.
         *
         */
        std::vector<Private::ComponentBase*> components[MAX_COMPONENTS];

        /**
         * @brief Contains recycled internal entity IDs.
         *
         * When creating a new entity, these IDs will be preferred.
         */
        std::vector<size_t> recycledIds;

        /**
         * @brief Keeps track of all entities to destroy.
         *
         * Removed entities are destroyed every time a system finishes processing.
         * Stores internal IDs.
         *
         */
        std::vector<size_t> entitiesToDestroy;

        /**
         * @brief Keeps track of all components to destroy.
         *
         * This vector holds tuples on the form (internalEntityId, componentType). This pair
         * can be used to find the component in the array.
         *
         * Removed components are destroyed every time a system finishes processing.
         */
        std::vector<std::pair<size_t, int>> componentsToDestroy;

        /**
         * @brief The UUID that will be given to the next entity.
         *
         */
        Entity nextUUID;

        /**
         * @brief The internal ID that will be given to the next entity if it cannot be recycled.
         *
         */
        size_t nextInternalId;
    };


    // IMPLEMENTATION

    template <typename T>
    T* EntityManager::AddComponent(Entity entity)
    {
        // TODO: Use a provided allocator instead of using new.

        // Find the internal ID of the entity.
        auto it = translator.find(entity);
        assert(it != translator.end());assert(it != translator.end());

        size_t internalId = it->second;
        assert(internalId < entities.size());
        assert(internalId < components[Component<T>::ID].size());

        // Create the new component.
        T* component = new T;
        components[Component<T>::ID][internalId] = component;
        entities[internalId].flags.set(Component<T>::ID, true);

        // TODO: Add to systems with matching aspects.

        return component;
    }

    template <typename T>
    T* EntityManager::GetComponent(Entity entity)
    {
        auto it = translator.find(entity);
        assert(it != translator.end());

        size_t internalId = it->second;
        assert(internalId < entities.size());
        assert(internalId < components[Component<T>::ID].size());

        return static_cast<T*>( components[Component<T>::ID][translator[entity]] );
    }

    template <typename T>
    void EntityManager::RemoveComponent(Entity entity)
    {
        auto it = translator.find(entity);
        assert(it != translator.end());

        size_t internalId = it->second;
        assert(internalId < entities.size());
        assert(internalId < components[Component<T>::ID].size());

        componentsToDestroy.push_back(std::make_pair(internalId, Component<T>::ID));
        entities[internalId].flags.set(Component<T>::ID, false);

        // TODO: Reconsider entity for all systems
    }

    template <typename T>
    bool EntityManager::HasComponent(Entity entity) const
    {
        auto it = translator.find(entity);
        assert(it != translator.end());

        size_t internalId = it->second;
        assert(internalId < entities.size());
        assert(internalId < components[Component<T>::ID].size());

        return components[Component<T>::ID][internalId] != nullptr;
    }

    template <typename T>
    bool EntityManager::IsComponentRemoved(Entity entity) const
    {
        auto it = translator.find(entity);
        assert(it != translator.end());

        size_t internalId = it->second;
        assert(internalId < entities.size());
        assert(internalId < components[Component<T>::ID].size());

        Private::ComponentBase* component = components[Component<T>::ID][internalId];
        return component != nullptr &&
               std::find(componentsToDestroy.begin(), componentsToDestroy.end(), component) != componentsToDestroy.end();
    }
}
