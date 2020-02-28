#pragma once
#include "GameObject.h"
#include "Component.h"

namespace CommonsLibrary
{
	class GameObject;
    class Scene
    {
        friend class World;
    private:
        std::vector<ReferencePointer<GameObject>> m_activeGameObjects;
        std::vector<ReferencePointer<GameObject>> m_inactiveGameObjects;

    public:
        Scene() = default;

        void Update(float deltaTime);

        ReferencePointer<GameObject> FindObject(const std::string& name);

        template <class Type>
        ReferencePointer<Type> FindObjectType()
        {
            ReferencePointer<Type> component = FindObjectType<Type>();
            return (component) ? component : FindObjectType<Type>();
        }
        template <class Type>
        std::vector<ReferencePointer<Type>> FindObjectsType()
        {
            std::vector<ReferencePointer<Type>> components;
            FindObjectsType<Type>(m_activeGameObjects, components);
            FindObjectsType<Type>(m_inactiveGameObjects, components);
            return components;
        }

    protected:
        virtual void LoadScene() = 0;

    private:
        void AddGameObject(ReferencePointer<GameObject> gameObject);
        void DeleteGameObject(const ReferencePointer<GameObject>& gameObject);

        void SetObjectActive(const ReferencePointer<GameObject>& gameObject);

    private:
        bool FindObjectToDelete(std::vector<ReferencePointer<GameObject>>& objectVector, const ReferencePointer<GameObject>& gameObject);
        ReferencePointer<GameObject> FindObject(const std::vector<ReferencePointer<GameObject>>& objectVector, const std::string& name);

        template <class Type>
        ReferencePointer<Type> FindObjectType(const std::vector<ReferencePointer<GameObject>>& objectVector)
        {
            for (const ReferencePointer<GameObject>& gameObject : objectVector)
            {
                ReferencePointer<Type> component = gameObject->GetComponent();
                if (component)
                    return component;
            }
            return nullptr;
        }

        template <class Type>
        void FindObjectsType(const std::vector<ReferencePointer<GameObject>>& objectVector, std::vector<ReferencePointer<Type>>& outputVector)
        {
            for (const ReferencePointer<GameObject>& gameObject : objectVector)
            {
                ReferencePointer<Type> component = gameObject->GetComponent();
                if (component)
                    return component;
            }
            return nullptr;
        }
    };
}