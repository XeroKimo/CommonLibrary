#include "CommonsLibrary/ECS/ECS.h"

namespace CommonsLibrary
{
    Scene::~Scene()
    {
        m_sceneDestroyed = true;
    }

    ReferenceView<Component> CommonsLibrary::Scene::CreateComponent(std::string type, const ReferenceView<GameObject>& gameObject)
    {
        auto it = m_componentManagerMap.find(type);
        if(it == m_componentManagerMap.end())
        {
            m_componentManagerMap.insert(ComponentRegistry::CreateComponentManager(type));
            it = m_componentManagerMap.find(type);
        }

        return it->second->CreateComponent(gameObject);
    }


   /* Scene::Scene(std::string name) :
        m_sceneName(name),
        m_rootGameObject(new GameObject())
    {
        m_rootGameObject->m_owningScene = this;
    }

    void Scene::Awake()
    {
        m_rootGameObject->Awake();
    }

    void Scene::Start()
    {
        TransferParents();
        DestroyGameObjects();
        TransferComponents();
    }

    void Scene::Update(float deltaTime)
    {
        m_rootGameObject->Update(deltaTime);
    }

    void Scene::TransferParents()
    {
        if(!m_transferParents.empty())
        {
            for(const auto& gameObject : m_transferParents)
            {
                gameObject->TransferParent();
            }

            m_transferParents.clear();
        }
    }

    void Scene::DestroyGameObjects()
    {
        if(!m_changeChildrenState.empty())
        {
            for(const auto& gameObject : m_changeChildrenState)
            {
                gameObject->ChangeChildrenState();
            }

            m_changeChildrenState.clear();
        }
    }

    void Scene::TransferComponents()
    {
        if(!m_changeComponentState.empty())
        {
            std::vector<ReferenceView<GameObject>> copy = m_changeComponentState;
            size_t startingSize = copy.size();

            for(size_t i = 0; i < startingSize; i++)
            {
                m_changeComponentState[i]->ChangeComponentsState();
            }

            m_changeComponentState.erase(m_changeComponentState.begin(), m_changeComponentState.begin() + startingSize);
        }
    }

    ReferenceView<GameObject> Scene::CreateGameObject(std::string name)
    {
        ReferenceView<GameObject> object = m_rootGameObject->CreateChild();
        object->name = name;
        if(m_isLoaded)
            object->Awake();

        return object;
    }
    void Scene::MergeScene(Scene* other)
    {
        if(m_isLoaded)
        {
            std::vector<ReferenceView<GameObject>> children = other->m_rootGameObject->GetChildren();
            for(auto child : children)
            {
                child->RequestParentChange(m_rootGameObject);
            }
            other->TransferParents();
        }
        else
        {
            m_rootGameObject.Swap(other->m_rootGameObject);
            m_changeComponentState.swap(other->m_changeComponentState);
            m_transferParents.swap(other->m_transferParents);
            m_changeChildrenState.swap(other->m_changeChildrenState);
            m_sceneName = other->m_sceneName;
        }
    }*/
}