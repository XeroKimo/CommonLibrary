#include "CommonsLibrary/ECS.h"
#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/DebugTools/Logger.h"
#include "CommonsLibrary/StdHelpers/VectorHelpers.h"

namespace CommonsLibrary
{
    GameObject::GameObject(Scene* const scene) :
        m_scene(scene),
        name("GameObject"),
        m_hasComponentToStart(false),
        m_hasComponentToRemove(false),
        m_isDestroyed(false),
        m_transform(nullptr),
        m_activeInHeirarchy(true),
        m_activeInWorld(true)
    {
    }

    void GameObject::Start()
    {
        if (m_activeInWorld)
            m_componentMap.Start();
    }
    void GameObject::Update(float deltaTime)
    {
        if (m_activeInWorld)
            m_componentMap.Update(deltaTime);
    }
    void GameObject::OnDestroy()
    {
        m_componentMap.OnDestroy();
    }

    void GameObject::RemoveComponent(const ReferencePointer<Component>& component)
    {
        if (!component)
            return;

        if (component->GetGameObject().Get() != this)
            component->GetGameObject()->RemoveComponent(component);
        else
        {
            m_componentMap.RemoveComponent(component);
        }
    }

    ReferencePointer<Transform> GameObject::GetTransform()
    {
        if (!m_transform)
        {
            ReferencePointer<Transform> transform = GetComponent<Transform>();
            if (transform)
                m_transform = transform;
            else
                m_transform = AddComponent<Transform>();
        }
        return m_transform;
    }

    void GameObject::SetActive(bool active)
    {
        if (m_activeInHeirarchy == active)
            return;

        m_activeInHeirarchy = active;
        SetActiveInWorld(IsParentActiveInWorld());
        SetChildrenActiveInWorld();
    }

    bool GameObject::GetActiveHeirarchy() const
    {
        return m_activeInHeirarchy;
    }

    bool GameObject::GetActiveWorld() const
    {
        return m_activeInWorld;
    }

    void GameObject::Destroy()
    {
        if (m_isDestroyed)
            return;
        m_componentMap.DestroyAllComponents();
        m_scene->DestroyGameObject(GetReferencePointer());
        m_isDestroyed = true;
    }

    void GameObject::SetChildrenActiveInWorld()
    {
        for (const auto& child : GetTransform()->GetChildren())
        {
            ReferencePointer<GameObject> gameObject = child->GetGameObject();
            if (gameObject->GetActiveHeirarchy())
            {
                gameObject->SetChildrenActiveInWorld();
            }
            gameObject->SetActiveInWorld(m_activeInWorld);
        }
    }

    bool GameObject::IsParentActiveInWorld()
    {
        ReferencePointer<Transform> parent = GetTransform()->GetParent();
        if (!parent)
            return m_activeInWorld;
        else
            return parent->GetGameObject()->IsParentActiveInWorld();
    }

    void GameObject::SetActiveInWorld(bool active)
    {
        active = active && m_activeInHeirarchy;
        if (m_activeInWorld == active)
            return;
        m_activeInWorld = active;
    }
    ReferencePointer<GameObject> GameObject::CreateGameObject()
    {
        return m_scene->CreateGameObject();
    }
}
