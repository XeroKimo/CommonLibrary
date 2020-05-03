#include "CommonsLibrary/ECS.h"

namespace CommonsLibrary
{
    Scene::Scene(std::string name) :
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
        if(!m_hierarchyStarts.empty())
        {
            for(auto gameObject : m_hierarchyStarts)
            {
                gameObject->StartHierarchy();
            }

            m_hierarchyStarts.clear();

        }
        if(!m_postStartCalls.empty())
        {
            for(auto gameObject : m_postStartCalls)
            {
                gameObject->PostStart();
            }

            m_postStartCalls.clear();
        }

        if(!m_componentStarts.empty())
        {
            auto copy = m_componentStarts;
            size_t startingSize = copy.size();

            for(size_t i = 0; i < startingSize; i++)
            {
                m_componentStarts[i]->StartComponents();
            }

            m_componentStarts.erase(m_componentStarts.begin(), m_componentStarts.begin() + startingSize);
        }
    }

    void Scene::Update(float deltaTime)
    {
        m_rootGameObject->Update(deltaTime);
    }

    ReferencePointer<GameObject> Scene::Instantiate(std::string name)
    {
        auto object = m_rootGameObject->CreateChild();
        object->name = name;
        if(m_isLoaded)
            object->Awake();

        return object;
    }
}