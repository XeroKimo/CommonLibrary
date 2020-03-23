#include "CommonsLibrary/ECS/GameObject.h"
#include "CommonsLibrary/ECS/World.h"
#include "CommonsLibrary/ECS/Scene.h"
#include "CommonsLibrary/DebugTools/Logger.h"
#include "CommonsLibrary/StdHelpers/VectorHelpers.h"
#include "CommonsLibrary/ECS/IGameObject.h"

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
        m_updateableComponents.Start();
        m_hasComponentToStart = false;
    }
    void GameObject::Update(float deltaTime)
    {
        m_updateableComponents.Update(deltaTime);
    }
    void GameObject::CleanUpComponents()
    {
        m_componentMap.CleanUp();
        m_hasComponentToRemove = false;
    }
    void GameObject::OnDestroy()
    {
        m_updateableComponents.OnGameObjectDestroyed();
    }

    ReferencePointer<Component> GameObject::AddComponent(const std::type_index& key)
    {
        ReferencePointer<Component> component = m_componentMap.AddComponent(GetReferencePointer(), key);
        m_updateableComponents.AddComponent(component.Get());

        bool wasInactive = !component->IsActive();
        if (wasInactive)
        {
            /*if (!component->m_hasStarted)
                componentsToStart.push_back(component);*/
        }
        return component;
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

    ReferencePointer<Component> GameObject::GetComponent(const std::type_index& key)
    {
        return m_componentMap.GetComponent(key);
    }

    std::vector<ReferencePointer<Component>> GameObject::GetComponents(const std::type_index& key)
    {
        return std::vector<ReferencePointer<Component>>();
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

    void GameObject::SetName(std::string_view name)
    {
        this->name = name;
    }

    std::string GameObject::GetName() const
    {
        return name;
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
        m_scene->DestroyGameObject(GetReferencePointer());
        m_isDestroyed = true;
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

    void GameObject::SetChildrenActiveInWorld()
    {
        for (const auto& child : GetTransform()->GetChildren())
        {
            ReferencePointer<GameObject> gameObject = ReferencePointerStaticCast<GameObject>(child->GetGameObject());
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
            return static_cast<GameObject*>(parent->GetGameObject().Get())->IsParentActiveInWorld();
    }

    void GameObject::SetActiveInWorld(bool active)
    {
        m_activeInWorld = active;
        m_scene->SetObjectActive(GetReferencePointer());
    }
}
