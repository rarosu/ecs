#pragma once

namespace ECS
{
    template<typename T> class Component;

    namespace Private
    {
        /**
         * @brief Private type. Keeps track of type IDs and allow for component polymorphism.
         */
        class ComponentBase
        {
            template <typename T> friend class ECS::Component;
        private:
            static int nextTypeId;
        };

        int ComponentBase::nextTypeId = 0;
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
        /**
         * @brief Type ID for the component. This is increased automatically for every instantiated type of the class.
         */
        static const int ID;
    protected:
        /**
         * @brief Protected constructor. Only inherited classes can be instantiated.
         *
         */
        Component() {}
    };

    // Increase the type ID for every template instantiation of a component.
    template <typename T>
    const int Component<T>::ID = Private::ComponentBase::nextTypeId++;
}