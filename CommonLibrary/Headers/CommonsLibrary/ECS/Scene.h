#pragma once
#include "GameObject.h"

namespace CommonsLibrary
{
    class World;
    class Scene
    {
        friend class SceneManager;
        friend class GameObject;
        friend class Component;
    private:
        std::vector<ReferencePointer<GameObject>> m_updateGameObjects;
        std::vector<ReferencePointer<GameObject>> m_gameObjectsToDestroy;

        std::string m_sceneName;

        World* m_world;

        bool m_active;
        bool m_loaded;
    public:
        Scene(std::string sceneName);
        Scene(Scene& other);
        ~Scene();

    public:
        ReferencePointer<GameObject> FindObject(const std::string& name) const;

        template <class Type>
        ReferencePointer<Type> FindObjectType() const
        {
            return FindObjectType<Type>(m_updateGameObjects);
        }
        template <class Type>
        std::vector<ReferencePointer<Type>> FindObjectsType() const
        {
            std::vector<ReferencePointer<Type>> components;
            FindObjectsType<Type>(m_updateGameObjects, components);
            return components;
        }

    public:
        std::string GetSceneName() const { return m_sceneName; }
        bool IsLoaded() const { return m_loaded; }
        bool IsActive() const { return m_active; }

    protected:
        ReferencePointer<GameObject> CreateGameObject();
        
        virtual void LoadScene() { }

    private:
        void CallLoadScene(World* world);

        void UnloadScene();

    private:
        void StartGameObjects();
        void UpdateGameObjects(float deltaTime);
        void DestroyGameObjects();

    private:
        void DestroyGameObject(const ReferencePointer<GameObject>& gameObject);

        void PlaceGameObject(ReferencePointer<GameObject> gameObject);
        ReferencePointer<GameObject> ExtractGameObject(const ReferencePointer<GameObject>& gameObject);

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