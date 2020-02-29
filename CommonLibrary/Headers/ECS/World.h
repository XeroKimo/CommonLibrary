#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "GameObject.h"
#include "Scene.h"
#include <memory>

namespace CommonsLibrary
{
    class World : public ReferenceFromThis<World>
    {
        friend class GameObject;
    private:
        std::vector<std::unique_ptr<Scene>> m_scenes;
        std::unordered_map<std::string, int> m_sceneIndices;
        std::vector<Scene*> m_loadedScenes;
        Scene* m_activeScene = nullptr;


        std::vector<Component*> m_componentsToStart;
    public:
        World(std::vector<std::unique_ptr<Scene>> scenes);
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