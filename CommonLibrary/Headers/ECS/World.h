#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "GameObject.h"
#include "Scene.h"
#include <memory>

namespace CommonsLibrary
{
    __interface ExternalSystem {};

    class World : public ReferenceFromThis<World>
    {
        friend class GameObject;
    private:
        std::unordered_map<std::type_index, ExternalSystem*> m_externalSystem;

        std::vector<std::unique_ptr<Scene>> m_scenes;
        std::unordered_map<std::string, int> m_sceneIndices;
        std::vector<Scene*> m_loadedScenes;
        Scene* m_activeScene = nullptr;

        std::vector<Component*> m_componentsToStart;

    public:
        World(std::vector<std::unique_ptr<Scene>> scenes);

    public:
        template <class Type, class Enable = std::enable_if_t<std::is_base_of_v<ExternalSystem, Type>>>
        void AddExternalSystem(Type* system) 
        {
            std::type_index key(typeid(Type));
            if (!KeyExists(m_externalSystems, key))
                m_externalSystems[key] = system;
        }

        template <class Type, class Enable = std::enable_if_t<std::is_base_of_v<ExternalSystem, Type>>>
        Type* GetExternalSystem() 
        {
            std::type_index key(typeid(Type));
            if (KeyExists(m_externalSystems, key)) 
                return static_cast<Type*>(m_externalSystems[key]);
            else
                return nullptr;
        }
    public:
        void Update(float deltaTime);

    public:
        ReferencePointer<GameObject> CreateGameObject();
        ReferencePointer<GameObject> CreateGameObject(const ReferencePointer<Transform>& parent);

        ReferencePointer<GameObject> FindObject(const std::string& name) const;

        template <class Type>
        ReferencePointer<Type> FindObjectType() const
        {
            for (const Scene* const scene : m_loadedScenes)
            {
                ReferencePointer<Type> component = scene->FindObjectType<Type>();
                if (component)
                    return component;
            }
            return nullptr;
        }
        template <class Type>
        std::vector<ReferencePointer<Type>> FindObjectsType() const
        {
            std::vector<ReferencePointer<Type>> components;
            for (const Scene* const scene : m_loadedScenes)
            {
                components.swap(scene->FindObjectsType<Type>());
            }
            return components;
        }

    public:
        void LoadScene(int index);

    private:
        void AddComponentToStart(Component* component);
        void RemoveComponentToStart(Component* component);

        void SetObjectActive(const ReferencePointer<GameObject>& gameObject);
    };
}