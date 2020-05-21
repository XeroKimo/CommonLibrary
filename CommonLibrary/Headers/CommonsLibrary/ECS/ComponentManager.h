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
        GameObject* m_gameObject;
        std::priority_queue<size_t> m_destroyedComponentsIndices;
        std::vector<ReferenceView<Component>> m_activeChangedComponents;

        size_t m_firstInactiveComponentIndex = 0;
        ComponentVector m_components;

        bool m_toldSceneChangeComponentsState = false;

    public:
        ComponentManager(GameObject* owner) : m_gameObject(owner) {}
        ~ComponentManager();

    public:
        void Awake();

        void ChangeComponentsState();
        void Update(float deltaTime);

    public:
        template<class Type>
        ReferenceView<Type> CreateComponent(const ReferenceView<GameObject>& gameObject, bool callAwake);

        template<class Type>
        ReferenceView<Type> GetComponent();
        template<class Type>
        std::vector<ReferenceView<Type>> GetComponents();

    public:
        void CopyComponents(const ReferenceView<GameObject>& gameObject, const ComponentManager& other);
        bool DestroyComponent(const ReferenceView<Component>& component);
        void SetComponentActive(const ReferenceView<Component>& component, bool active);

        bool HasStartFlagsSet() const { return !m_destroyedComponentsIndices.empty() || !m_activeChangedComponents.empty(); }

        void AddToStartCall();
    private:
        ReferenceView<Component> CreateComponent(const ReferenceView<GameObject>& gameObject, bool callAwake, std::type_index type);

        template<class Type>
        ReferenceView<Type> GetComponent(const std::vector<ReferenceView<Component>>& componentVector);

        template<class Type>
        void GetComponents(const std::vector<ReferenceView<Component>>& componentVector, std::vector<ReferenceView<Component>>& outVector);

    private:
        void ClearDestroyedComponents();
        void TransferComponents();

        void SwapComponentActive(size_t componentIndex);
        void SwapComponents(size_t lh, size_t rh);

        ReferenceView<Component> Copy(const ReferenceView<GameObject>& gameObject, const ReferenceView<Component>& component);

    private:

        void RecountIndicesStarting(size_t startIndex);
    };





    template<class Type>
    inline ReferenceView<Type> ComponentManager::CreateComponent(const ReferenceView<GameObject>& gameObject, bool callAwake)
    {
        auto component = CreateComponent(gameObject, callAwake, typeid(Type));
        return component.StaticCast<Type>();
    }

    template<class Type>
    inline ReferenceView<Type> ComponentManager::GetComponent()
    {
        std::type_index originalKey(typeid(Type));
        std::type_index currentKey(typeid(Component));

        ReferencePointer<Type> found = GetComponent<Type>(m_components);

        return found;
    }

    template<class Type>
    inline std::vector<ReferenceView<Type>> ComponentManager::GetComponents()
    {
        std::vector<Type> output;

        GetComponents<Type>(m_components, output);

        return output;
    }

    template<class Type>
    inline ReferenceView<Type> ComponentManager::GetComponent(const std::vector<ReferenceView<Component>>& componentVector)
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
    inline void ComponentManager::GetComponents(const std::vector<ReferenceView<Component>>& componentVector, std::vector<ReferenceView<Component>>& outVector)
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