#pragma once
#include "GameObject.h"

namespace CommonsLibrary
{
    class World;
    class Scene
    {
        friend class GameObject;
    private:
        ReferencePointer<GameObject> m_rootGameObject;
        std::string m_sceneName;

        std::vector<ReferencePointer<GameObject>> m_componentStarts;
        std::vector<ReferencePointer<GameObject>> m_hierarchyStarts;
        std::vector<ReferencePointer<GameObject>> m_postStartCalls;

        bool m_isLoaded = false;
    public:
        Scene(std::string name);

    public:
        void Awake();
        void Start();
        void Update(float deltaTime);

    public:
        ReferencePointer<GameObject> Instantiate(std::string name);

    private:
        ReferencePointer<GameObject> GetRootGameObject() { return m_rootGameObject; }
    };
}