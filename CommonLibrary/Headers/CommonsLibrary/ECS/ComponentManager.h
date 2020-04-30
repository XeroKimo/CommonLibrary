#pragma once
#include "Component.h"
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <unordered_set>

namespace CommonsLibrary
{
    class ComponentManager
    {
        using ComponentVector = std::vector<ReferencePointer<Component>>;

    private:
        size_t m_hasDestroyInInactive = 0;
        size_t m_hasDestroyInActive = 0;
        size_t m_hasActiveChangedToInactive = 0;
        size_t m_hasActiveChangedToActive = 0;

        ComponentVector m_activeComponents;
        ComponentVector m_inactiveComponents;

        bool m_hasStartComponents = false;

    public:
        ~ComponentManager();

    public:
        void Awake();

        void TransferComponents();
        void Start();
        void Update(float deltaTime);
        void ClearDestroyedComponents();

    public:
        ReferencePointer<Component> CreateComponent(const ReferencePointer<GameObject>& gameObject, bool callAwake, std::type_index type);

        template<class Type>
        ReferencePointer<Type> CreateComponent(const ReferencePointer<GameObject>& gameObject, bool callAwake);

        template<class Type>
        ReferencePointer<Type> GetComponent(bool includeInactive = false);
        template<class Type>
        std::vector<ReferencePointer<Type>> GetComponents(bool includeInactive = false);

        bool DestroyComponent(const ReferencePointer<Component>& component);
        void SetComponentActive(const ReferencePointer<Component>& component, bool active);

        bool HasPreUpdateFlagsSet() const { return (m_hasStartComponents || (m_hasActiveChangedToActive + m_hasActiveChangedToInactive) > 0); }
        bool HasPostUpdateFlagsSet() const { return (m_hasDestroyInInactive + m_hasDestroyInActive) > 0; }
    public:
        void CopyComponents(const ReferencePointer<GameObject>& gameObject, const ComponentManager& other);

    private:
        template<class Type>
        ReferencePointer<Type> GetComponent(const std::vector<ReferencePointer<Component>>& componentVector);

        template<class Type>
        void GetComponents(const std::vector<ReferencePointer<Component>>& componentVector, std::vector<ReferencePointer<Component>>& outVector);

    private:
        void CallDestroyAll(std::vector<ReferencePointer<Component>>& componentVector);

        void TransferComponents(ComponentVector& from, ComponentVector& to);
        void DestroyComponents(ComponentVector& components);

        void ClearNullComponents(ComponentVector& componentVector);

        ReferencePointer<Component> Copy(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<Component>& component);
    };





    template<class Type>
    inline ReferencePointer<Type> ComponentManager::CreateComponent(const ReferencePointer<GameObject>& gameObject, bool callAwake)
    {
        return CreateComponent(gameObject, callAwake, typeid(Type)).StaticCast<Type>();
    }

    template<class Type>
    inline ReferencePointer<Type> ComponentManager::GetComponent(bool includeInactive)
    {
        std::type_index originalKey(typeid(Type));
        std::type_index currentKey(typeid(Component));

        ReferencePointer<Type> found = GetComponent<Type>(m_activeComponents);
        if(!found && includeInactive)
            found = GetComponent<Type>(m_inactiveComponents);

        return found;
    }

    template<class Type>
    inline std::vector<ReferencePointer<Type>> ComponentManager::GetComponents(bool includeInactive)
    {
        std::vector<Type> output;

        GetComponents<Type>(m_activeComponents, output);
        if(includeInactive)
            GetComponents<Type>(m_inactiveComponents, output);


        return output;
    }

    template<class Type>
    inline ReferencePointer<Type> ComponentManager::GetComponent(const std::vector<ReferencePointer<Component>>& componentVector)
    {
        if(componentVector.empty())
            return nullptr;

        std::unordered_set<std::type_index> comparedKeys;
        ReferencePointer<Type> cast;
        for(auto component : componentVector)
        {
            if(comparedKeys.count(typeid(*component)) != 0)
                continue;
            comparedKeys.insert(typeid(*component));
            cast = ReferencePointerDynamicCast<Type>(component);
            if(cast)
                return cast;
        }

        return cast;
    }

    template<class Type>
    inline void ComponentManager::GetComponents(const std::vector<ReferencePointer<Component>>& componentVector, std::vector<ReferencePointer<Component>>& outVector)
    {
        if(componentVector.empty())
            return;

        std::unordered_map<std::type_index, bool> comparedKeys;
        ReferencePointer<Type> cast;
        for(auto component : componentVector)
        {
            if(comparedKeys.count(typeid(*component)) != 0)
            {
                if(!comparedKeys[typeid(*component)])
                    continue;

                outVector.push_back(ReferencePointerStaticCast<Type>(component));
            }
            else
                comparedKeys[typeid(*component)] = false;

            cast = ReferencePointerDynamicCast<Type>(component);
            if(cast)
            {
                outVector.push_back(cast);
                comparedKeys[typeid(*component)] = false;
            }
        }

    }

}