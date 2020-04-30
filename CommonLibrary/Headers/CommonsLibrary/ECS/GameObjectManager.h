#pragma once
#include "GameObject.h"
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <unordered_set>

namespace CommonsLibrary
{
    class GameObjectManager
    {
        using GameObjectVector = std::vector<ReferencePointer<GameObject>>;

    private:
        GameObjectVector m_activeGameObjects;
        GameObjectVector m_inactiveGameObjects;

    public:
        void Awake();

        void TransferObjects();

        void PreUpdate();
        void Update(float deltaTime);
        void PostUpdate();

        void ClearDestroyedObjects();
    public:
        ReferencePointer<Component> CreateComponent(const ReferencePointer<GameObject>& gameObject, bool objectConstructed, bool sceneLoaded, std::type_index type);

        template<class Type>
        ReferencePointer<Type> CreateComponent(const ReferencePointer<GameObject>& gameObject, bool objectConstructed, bool sceneLoaded);

        template<class Type>
        ReferencePointer<Type> GetComponent(bool includeInactive = false);
        template<class Type>
        std::vector<ReferencePointer<Type>> GetComponents(bool includeInactive = false);

        bool DestroyComponent(const ReferencePointer<Component>& component);
        void SetActiveInHeirarchy(const ReferencePointer<GameObject>& gameObject, bool active);
        void SetActiveInWorld(const ReferencePointer<GameObject>& gameObject, bool active);

    public:
        void CopyComponents(const ReferencePointer<GameObject>& gameObject, bool objectConstructed, bool sceneLoaded, const ComponentManager& other);

    private:
        template<class Type>
        ReferencePointer<Type> GetComponent(const std::vector<ReferencePointer<Component>>& componentVector);

        template<class Type>
        void GetComponents(const std::vector<ReferencePointer<Component>>& componentVector, std::vector<ReferencePointer<Component>>& outVector);

    private:
        void CallDestroyAll(GameObjectVector& componentVector);

        GameObjectVector BeginTransferComponents();
        void ClearTransferedComponents();
        void MoveStartComponents(GameObjectVector& startVector);

        void ClearNullComponents(GameObjectVector& componentVector);
    };





    //template<class Type>
    //inline ReferencePointer<Type> ComponentManager::CreateComponent(const ReferencePointer<GameObject>& gameObject, bool objectConstructed, bool sceneLoaded)
    //{
    //    std::type_index key(typeid(Type));

    //    m_hasStartComponents = true;
    //    m_hasActiveChangedComponents = true;

    //    m_inactiveComponents.push_back(ComponentRegistry::CreateComponent(typeid(Type), gameObject));
    //    if(objectConstructed && sceneLoaded)
    //        m_inactiveComponents.back()->Awake();
    //    m_bufferComponents.push_back(m_inactiveComponents.back());

    //    return ReferencePointerStaticCast<Type>(m_inactiveComponents.back());
    //}

    //template<class Type>
    //inline ReferencePointer<Type> ComponentManager::GetComponent(bool includeInactive)
    //{
    //    std::type_index originalKey(typeid(Type));
    //    std::type_index currentKey(typeid(Component));

    //    ReferencePointer<Type> found = GetComponent<Type>(m_activeComponents);
    //    if(!found)
    //        found = GetComponent<Type>(m_bufferComponents);
    //    if(!found && includeInactive)
    //        found = GetComponent<Type>(m_inactiveComponents);

    //    return found;
    //}

    //template<class Type>
    //inline std::vector<ReferencePointer<Type>> ComponentManager::GetComponents(bool includeInactive)
    //{
    //    std::vector<Type> output;

    //    GetComponents<Type>(m_activeComponents, output);
    //    GetComponents<Type>(m_bufferComponents, output);
    //    if(includeInactive)
    //        GetComponents<Type>(m_inactiveComponents, output);


    //    return output;
    //}

    //template<class Type>
    //inline ReferencePointer<Type> ComponentManager::GetComponent(const std::vector<ReferencePointer<Component>>& componentVector)
    //{
    //    if(componentVector.empty())
    //        return nullptr;

    //    std::unordered_set<std::type_index> comparedKeys;
    //    ReferencePointer<Type> cast;
    //    for(auto component : componentVector)
    //    {
    //        if(comparedKeys.count(typeid(*component)) != 0)
    //            continue;
    //        comparedKeys.insert(typeid(*component));
    //        cast = ReferencePointerDynamicCast<Type>(component);
    //        if(cast)
    //            return cast;
    //    }

    //    return cast;
    //}

    //template<class Type>
    //inline void ComponentManager::GetComponents(const std::vector<ReferencePointer<Component>>& componentVector, std::vector<ReferencePointer<Component>>& outVector)
    //{
    //    if(componentVector.empty())
    //        return;

    //    std::unordered_map<std::type_index, bool> comparedKeys;
    //    ReferencePointer<Type> cast;
    //    for(auto component : componentVector)
    //    {
    //        if(comparedKeys.count(typeid(*component)) != 0)
    //        {
    //            if(!comparedKeys[typeid(*component)])
    //                continue;

    //            outVector.push_back(ReferencePointerStaticCast<Type>(component));
    //        }
    //        else
    //            comparedKeys[typeid(*component)] = false;

    //        cast = ReferencePointerDynamicCast<Type>(component);
    //        if(cast)
    //        {
    //            outVector.push_back(cast);
    //            comparedKeys[typeid(*component)] = false;
    //        }
    //    }

    //}

}