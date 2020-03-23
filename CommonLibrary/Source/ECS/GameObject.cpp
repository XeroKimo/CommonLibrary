#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/ECS/World.h"
#include "CommonsLibrary/ECS/Scene.h"
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
        m_transform(nullptr)
    {
    }

    void GameObject::SetIsActive(bool active)
    {
        if (m_activeInHeirarchy == active)
            return;

        m_activeInHeirarchy = active;
        SetActiveInWorld(IsParentActiveInWorld());
        SetChildrenActiveInWorld();
    }

    void GameObject::Destroy()
    {
        if (m_isDestroyed)
            return;
        m_scene->DestroyGameObject(GetReferencePointer());
        m_isDestroyed = true;
    }


    void GameObject::RemoveComponent(const ReferencePointer<Component>& component)
    {
        if (!component)
            return;
        //if component has been flagged for destroy
        //return ;

        if (component->GetGameObject().Get() != this)
            component->GetGameObject()->RemoveComponent(component);
        else
        {
            m_updateableComponents.RemoveComponent(std::move(m_componentMap.RemoveComponent(component)));
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
    void GameObject::AddGameObjectToStart()
    {
        if (m_hasComponentToStart)
            return;
        m_scene->SetGameObjectToStart(GetReferencePointer());
        m_hasComponentToStart = true;
    }
    void GameObject::AddGameObjectToCleanUp()
    {
        m_hasComponentToRemove = true;
        //m_scene->AddGameObject(GetReferencePointer());
    }
    void GameObject::SetComponentActive(const ReferencePointer<Component>& component)
    {
        m_updateableComponents.SetComponentActive(component.Get());

        bool wasInactive = !component->IsActive();
        if (wasInactive)
        {
            if (component->m_hasStarted)
            {
                AddGameObjectToStart();
            }
        }
    }

    void GameObject::SetChildrenActiveInWorld()
    {
        for (const auto& child : GetTransform()->GetChildren())
        {
            ReferencePointer<GameObject> gameObject = child->GetGameObject();
            if (gameObject->GetActiveHeirarchy())
            {
                SetChildrenActiveInWorld();
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
        m_activeInWorld = active;
        m_scene->SetObjectActive(GetReferencePointer());
    }
    UpdateableGameObject::UpdateableGameObject(Scene* const scene) :
        GameObject(scene)
    {
    }
    void UpdateableGameObject::Start()
    {
        m_updateableComponents.Start();
        m_hasComponentToStart = false;
    }
    void UpdateableGameObject::Update(float deltaTime)
    {
        m_updateableComponents.Update(deltaTime);
    }
    void UpdateableGameObject::CleanUpComponents()
    {
        m_updateableComponents.CleanUp();
        m_hasComponentToRemove = false;
    }
    void UpdateableGameObject::OnDestroy()
    {
        m_updateableComponents.OnGameObjectDestroyed();
    }
}
