#pragma once
#include "GameObject.h"

namespace CommonsLibrary
{
    class World;
    class Scene
    {
        friend class GameObject;
        friend class SceneManager;

    private:
        ReferencePointer<GameObject> m_rootGameObject;
        std::string m_sceneName;

        std::vector<ReferencePointer<GameObject>> m_changeComponentState;
        std::vector<ReferencePointer<GameObject>> m_transferParents;
        std::vector<ReferencePointer<GameObject>> m_changeChildrenState;

        bool m_isLoaded = false;
    private:
        Scene(std::string name);

    private:
        void Awake();
        void Start();
        void Update(float deltaTime);

    private:
        void TransferParents();
        void DestroyGameObjects();
        void TransferComponents();

    private:
        ReferencePointer<GameObject> CreateGameObject(std::string name);
        void MergeScene(Scene* other);

        ReferencePointer<GameObject> GetRootGameObject() { return m_rootGameObject; }

    protected:
        virtual void Load() {};
    };
}