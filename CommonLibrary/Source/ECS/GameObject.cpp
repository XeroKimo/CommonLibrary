#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/DebugTools/Logger.h"
#include "CommonsLibrary/StdHelpers/VectorHelpers.h"

namespace CommonsLibrary
{
    void GameObject::Awake()
    {
        m_componentManager.Awake();
    }
    void GameObject::PreUpdate()
    {
        m_componentManager.TransferComponents();
        m_componentManager.Start();

        m_hierarchy.PreUpdate();
    }

    void GameObject::Update(float deltaTime)
    {
        m_componentManager.Update(deltaTime);
        m_hierarchy.Update(deltaTime);
    }

    void GameObject::PostUpdate()
    {
        m_componentManager.ClearDestroyedComponents();
        m_hierarchy.PostUpdate();
    }

    void GameObject::SetActive(bool active)
    {
    }

    void GameObject::SetActiveWorld(bool active)
    {

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



    void DestroyGameObject(const ReferencePointer<GameObject>& component)
    {
    }


    void DestroyGameObject(const ReferencePointer<Component>& component)
    {
    }

    void DestroyComponent(const ReferencePointer<Component>& component)
    {
        if(typeid(*component) == typeid(Transform))
            return;
        component->GetGameObject()->DestroyComponent(component);
    }

}
