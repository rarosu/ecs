#pragma once

namespace ECS
{
    template<typename T> class Component;

    typedef unsigned int ComponentType;

    namespace Private
    {
        /**
         * @brief Private type. Keeps track of type IDs and allow for component polymorphism.
         */
        class ComponentBase
        {
            template <typename T> friend class ECS::Component;
        public:
            virtual ~ComponentBase() {}
        protected:
            /**
             * @brief Protected constructor. Only inherited classes can be instantiated.
             *
             */
            ComponentBase() {}
        private:
            /**
             * @brief Increased for every instantiated type of Component.
             *
             */
            static ComponentType nextTypeId;
        };
    }

    /**
     * @brief A component is a collection of data that can be associated with an entity.
     *
     * Inherit from this class to create your own component types. Pass along the inherited type
     * as the template parameter.
     *
     */
    template <typename T>
    class Component : public Private::ComponentBase
    {
    public:
        virtual ~Component() {}

        /**
         * @brief Type ID for the component. This is increased automatically for every instantiated type of the class.
         */
        static const ComponentType ID;
    protected:
        /**
         * @brief Protected constructor. Only inherited classes can be instantiated.
         *
         */
        Component() {}
    };

    // Increase the type ID for every template instantiation of a component.
    template <typename T>
    const ComponentType Component<T>::ID = Private::ComponentBase::nextTypeId++;
}
