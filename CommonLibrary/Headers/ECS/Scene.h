#pragma once
#include "GameObject.h"
#include "Component.h"

namespace CommonsLibrary
{
	class GameObject;
    class World;
    class Scene
    {
        friend class World;
    private:
        std::vector<ReferencePointer<GameObject>> m_activeGameObjects;
        std::vector<ReferencePointer<GameObject>> m_inactiveGameObjects;

        std::string m_sceneName;
    public:
        Scene(std::string sceneName);

        void Update(float deltaTime);

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

        std::string GetSceneName() { return m_sceneName; }
    protected:
        virtual void LoadScene(const ReferencePointer<World>& world) = 0;
    private:
        void AddGameObject(ReferencePointer<GameObject> gameObject);
        void DeleteGameObject(const ReferencePointer<GameObject>& gameObject);

        void SetObjectActive(const ReferencePointer<GameObject>& gameObject);

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
                    return component;
            }
            return nullptr;
        }
    };
}