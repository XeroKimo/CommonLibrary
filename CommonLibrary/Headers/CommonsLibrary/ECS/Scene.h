#pragma once
#include "GameObject.h"

namespace CommonsLibrary
{
    class World;
    class Scene
    {
        friend class SceneManager;
        friend class GameObject;
    private:
        std::vector<ReferencePointer<GameObject>> m_activeGameObjects;
        std::vector<ReferencePointer<GameObject>> m_inactiveGameObjects;
        std::vector<ReferencePointer<GameObject>> m_gameObjectsToDestroy;
        std::vector<ReferencePointer<GameObject>> m_gameObjectsToStart;

        std::string m_sceneName;

        World* m_world;
    public:
        Scene(std::string sceneName);
        Scene(Scene& other);
        ~Scene();

    public:
        ReferencePointer<GameObject> CreateGameObject();
        ReferencePointer<GameObject> FindObject(const std::string& name) const;

        template <class Type>
        ReferencePointer<Type> FindObjectType() const
        {
            ReferencePointer<Type> component = FindObjectType<Type>(m_activeGameObjects);
            return (component) ? component : FindObjectType<Type>(m_inactiveGameObjects);
        }
        template <class Type>
        std::vector<ReferencePointer<Type>> FindObjectsType() const
        {
            std::vector<ReferencePointer<Type>> components;
            FindObjectsType<Type>(m_activeGameObjects, components);
            FindObjectsType<Type>(m_inactiveGameObjects, components);
            return components;
        }

    public:
        std::string GetSceneName() { return m_sceneName; }

        World* GetWorld() { return m_world; }

    protected:
        virtual void LoadScene(World* world) { m_world = world; }

    private:
        void UnloadScene();

    private:
        void StartGameObjects();
        void UpdateGameObjects(float deltaTime);
        void DestroyGameObjects();

    private:
        void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);
        void SetObjectActive(const ReferencePointer<GameObject>& gameObject);
        void SetGameObjectToStart(const ReferencePointer<GameObject>& gameObject);

    private:
        bool FindObjectToDelete(std::vector<ReferencePointer<GameObject>>& objectVector, const ReferencePointer<GameObject>& gameObject);
        ReferencePointer<GameObject> FindObject(const std::vector<ReferencePointer<GameObject>>& objectVector, const std::string& name) const;

        template <class Type>
        ReferencePointer<Type> FindObjectType(const std::vector<ReferencePointer<GameObject>>& objectVector) const
        {
            for (const ReferencePointer<GameObject>& gameObject : objectVector)
            {
                ReferencePointer<Type> component = gameObject->GetComponent<Type>();
                if (component)
                    return component;
            }
            return nullptr;
        }

        template <class Type>
        void FindObjectsType(const std::vector<ReferencePointer<GameObject>>& objectVector, std::vector<ReferencePointer<Type>>& outputVector) const
        {
            for (const ReferencePointer<GameObject>& gameObject : objectVector)
            {
                ReferencePointer<Type> component = gameObject->GetComponent<Type>();
                if (component)
                    outputVector.push_back(component);
            }
        }
    };
}