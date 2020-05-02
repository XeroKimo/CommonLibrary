#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/DebugTools/Logger.h"
#include "CommonsLibrary/StdHelpers/VectorHelpers.h"

namespace CommonsLibrary
{
    void GameObject::PreAwake()
    {
        m_hierarchy.PreAwake();
    }
    void GameObject::Awake()
    {
        m_componentManager.Awake();
        m_hierarchy.Awake();
    }

    void GameObject::Start()
    {
        m_componentManager.Start();
    }

    void GameObject::Update(float deltaTime)
    {
        m_componentManager.Update(deltaTime);
        m_hierarchy.Update(deltaTime);

        if(m_hierarchy.HasPreUpdateFlagsSet())
            m_hierarchy.SetPreUpdateFlag();
        if(m_hierarchy.HasPostUpdateFlagsSet())
            m_hierarchy.SetPostUpdateFlag();
    }

    void GameObject::SetActive(bool active)
    {
        m_hierarchy.GetParent()->m_hierarchy.SetActive(GetReferencePointer(), active);
    }

    ReferencePointer<GameObject> GameObject::Construct()
    {
        ReferencePointer<GameObject> newObject = MakeReference<GameObject>();
        return newObject;
    }

    ReferencePointer<GameObject> GameObject::CopyConstruct(const ReferencePointer<GameObject>& other)
    {
        ReferencePointer<GameObject> newObject = MakeReference<GameObject>();

        newObject->name = other->name;
        newObject->CopyComponents(other->m_componentManager);

        return newObject;
    }



    void DestroyGameObject(const ReferencePointer<GameObject>& gameObject)
    {
        gameObject->GetParent()->m_hierarchy.DestroyGameObject(gameObject);
    }


    void DestroyGameObject(const ReferencePointer<Component>& component)
    {
        DestroyGameObject(component->GetGameObject());
    }

    void DestroyComponent(const ReferencePointer<Component>& component)
    {
        if(typeid(*component) == typeid(Transform))
            return;
        component->GetGameObject()->DestroyComponent(component);
    }

}
