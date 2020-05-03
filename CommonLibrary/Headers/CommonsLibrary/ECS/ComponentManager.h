#pragma once
#include "Component.h"
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <unordered_set>
#include <queue>

namespace CommonsLibrary
{
    class ComponentManager
    {
        using ComponentVector = std::vector<ReferencePointer<Component>>;

    private:
        GameObject* m_owner;
        std::priority_queue<size_t> m_destroyedComponentsIndices;
        std::vector<size_t*> m_activeChangedIndices;

        size_t m_firstInactiveComponentIndex = 0;
        ComponentVector m_components;

        bool m_toldSceneToCallStart = false;

    public:
        ComponentManager(GameObject* owner) : m_owner(owner) {}
        ~ComponentManager();

    public:
        void Awake();

        void Start();
        void Update(float deltaTime);

    public:
        template<class Type>
        ReferencePointer<Type> CreateComponent(const ReferencePointer<GameObject>& gameObject, bool callAwake);

        template<class Type>
        ReferencePointer<Type> GetComponent();
        template<class Type>
        std::vector<ReferencePointer<Type>> GetComponents();

    public:
        void CopyComponents(const ReferencePointer<GameObject>& gameObject, const ComponentManager& other);
        bool DestroyComponent(const ReferencePointer<Component>& component);
        void SetComponentActive(const ReferencePointer<Component>& component, bool active);

        bool HasStartFlagsSet() const { return !m_destroyedComponentsIndices.empty() || !m_activeChangedIndices.empty(); }

        void AddToStartCall();
    private:
        ReferencePointer<Component> CreateComponent(const ReferencePointer<GameObject>& gameObject, std::type_index type);

        template<class Type>
        ReferencePointer<Type> GetComponent(const std::vector<ReferencePointer<Component>>& componentVector);

        template<class Type>
        void GetComponents(const std::vector<ReferencePointer<Component>>& componentVector, std::vector<ReferencePointer<Component>>& outVector);

    private:
        void ClearDestroyedComponents();
        void TransferComponents();

        void SwapComponentActive(size_t componentIndex);
        void SwapComponents(size_t lh, size_t rh);

        ReferencePointer<Component> Copy(const ReferencePointer<GameObject>& gameObject, const ReferencePointer<Component>& component);

    private:

        void RecountIndicesStarting(size_t startIndex);
    };





    template<class Type>
    inline ReferencePointer<Type> ComponentManager::CreateComponent(const ReferencePointer<GameObject>& gameObject, bool callAwake)
    {
        auto component = CreateComponent(gameObject, typeid(Type));

        if(callAwake)
            component->Awake();
        if(component->m_active)
            m_activeChangedIndices.push_back(&component->m_componentIndex);

        return component.StaticCast<Type>();
    }

    template<class Type>
    inline ReferencePointer<Type> ComponentManager::GetComponent()
    {
        std::type_index originalKey(typeid(Type));
        std::type_index currentKey(typeid(Component));

        ReferencePointer<Type> found = GetComponent<Type>(m_components);

        return found;
    }

    template<class Type>
    inline std::vector<ReferencePointer<Type>> ComponentManager::GetComponents()
    {
        std::vector<Type> output;

        GetComponents<Type>(m_components, output);

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