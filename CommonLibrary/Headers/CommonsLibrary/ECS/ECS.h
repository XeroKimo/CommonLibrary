#pragma once
#include "../Miscellaneous/ReferencePointer.h"
#include <unordered_map>
#include <map>
#include <typeindex>
#include <queue>

namespace CommonsLibrary
{
    class Component;
    class ComponentManagerBase;
    template<class Type>
    class ComponentManager;

    class Transform;
    class GameObject;
    class GameObjectManager;
    class Scene;
    class SceneManager;

    namespace ComponentRegistry
    {
        std::unordered_map<std::string, std::unique_ptr<ComponentManagerBase>>& GetRegisteredComponents();

        template <class Type, std::enable_if_t<std::is_base_of_v<Component, Type>, int> = 0>
        inline void RegisterComponent()
        {
            if constexpr(std::negation<std::is_abstract<Type>>::value)
                GetRegisteredComponents()[typeid(Type).name()] = std::make_unique<ComponentManager<Type>>();
        }

        std::pair<std::string, std::unique_ptr<ComponentManagerBase>> CreateComponentManager(std::type_index type);
        std::pair<std::string, std::unique_ptr<ComponentManagerBase>> CreateComponentManager(std::string type);
    };



    class Component : ReferenceViewEnableThis<Component>
    {
        friend class ComponentManagerBase;
        template<class Type>
        friend class ComponentManager;
        friend class GameObject;

        bool m_active = true;
        bool m_previousActive = false;

        bool m_gameObjectActiveWorld = true;

        bool m_isDestroyed = false;
        bool m_hasStarted = false;
        bool m_hasRequestedActiveChanged = true;

        size_t m_componentIndex = 0;

        ReferenceView<GameObject> m_gameObject;

    public:
        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void OnDestroyed() {}

    public:
        ReferenceView<GameObject> GetGameObject() { return m_gameObject; }

    };



    class ComponentManagerBase
    {
    protected:
        size_t m_firstInactiveComponent = 0;

        std::vector<ReferencePointer<Component>> m_managedComponents;

        std::priority_queue<size_t> m_destroyedComponentIndices;
        std::vector<ReferenceView<Component>> m_activeChangedComponents;
    public:
        virtual ~ComponentManagerBase() = default;

        void ClearDestroyedComponents();
        virtual void UpdateComponentStates() = 0;
        virtual void Update(float deltaTime) = 0;

        virtual ComponentManagerBase* CreateCopy() = 0;
        virtual ReferenceView<Component> CreateComponent(const ReferenceView<GameObject>& gameObject) = 0;

        void DestroyComponent(const ReferenceView<Component>& component);
        void SetComponentActive(const ReferenceView<Component>& component, bool active);
        void SetComponentActiveWorld(const ReferenceView<Component>& component, bool active);
    protected:
        void SwapComponents(size_t lh, size_t rh);
    };



    template<class Type>
    class ComponentManager final : public ComponentManagerBase
    {
    public:
        ~ComponentManager() override
        {
            ClearDestroyedComponents();
            for(const auto& component : m_managedComponents)
            {
                static_cast<Type*>(component.Get())->Type::OnDestroyed();
            }
        }
        void UpdateComponentStates() override
        {
            if(m_activeChangedComponents.empty())
                return;

            for(const auto& component : m_activeChangedComponents)
            {
                if(!component)
                    continue;

                component->m_hasRequestedActiveChanged = false;

                if(!ComponentActiveChanged(component))
                    continue;

                component->m_previousActive = component->m_active;

                if(component->m_active && component->m_gameObjectActiveWorld)
                {
                    if(!component->m_hasStarted)
                        static_cast<Type*>(component.Get())->Type::Start();
                    SwapComponents(m_firstInactiveComponent, component->m_componentIndex);

                    m_firstInactiveComponent++;
                }
                else
                {
                    m_firstInactiveComponent--;
                    SwapComponents(m_firstInactiveComponent, component->m_componentIndex);
                }
            }

            m_activeChangedComponents.clear();
        }

        void Update(float deltaTime) override
        {
            std::for_each
            (
                m_managedComponents.begin(),
                m_managedComponents.begin() + m_firstInactiveComponent,
                [deltaTime](const ReferencePointer<Component>& value) { static_cast<Type*>(value.Get())->Type::Update(deltaTime); }
            );
        }

        ComponentManager* CreateCopy() override
        {
            return new ComponentManager();
        }
        ReferenceView<Component> CreateComponent(const ReferenceView<GameObject>& gameObject) override
        {
            size_t currentIndex = m_managedComponents.size();
            m_managedComponents.emplace_back(MakeReference<Type>());
            m_managedComponents.back()->m_gameObject = gameObject;
            m_managedComponents.back()->m_componentIndex = currentIndex;
            m_activeChangedComponents.push_back(m_managedComponents.back());

            return ReferenceView<Component>(m_managedComponents[m_firstInactiveComponent++]);
        }

    private:
        bool ComponentActiveChanged(const ReferenceView<Component> component)
        {
            return component->m_active != component->m_previousActive;
        }
    };



    class Transform
    {
    };



    class GameObject final : ReferenceViewEnableThis<GameObject>
    {
        friend class Scene;
        friend class GameObjectManager;
    private:
        Scene* m_owningScene = nullptr;
        GameObject* m_parent = nullptr;
        std::vector<GameObject*> m_children;
        std::map<std::string, std::vector<ReferenceView<Component>>> m_componentMap;

        size_t m_childIndex = 0;
        size_t m_sceneIndex = 0;
        Transform m_transform;

        bool m_active = true;
        bool m_isDestroyed = false;
        bool m_activeChanged = false;
        bool m_hasRequestedActiveChanged = false;
        bool m_finishedConstruction = false;

    public:
        std::string name = "New GameObject";

    public:
        GameObject() = default;
        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) noexcept = default;

        GameObject operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) noexcept = default;

        ~GameObject();

    public:
        void Awake();

        template<class Type>
        ReferenceView<Type> AddComponent()
        {
            return AddComponent(typeid(Type)).StaticCast<Type>();
        }
        ReferenceView<Component> AddComponent(std::type_index type);

        bool IsActiveSelf() const { return m_active; }
        bool IsActiveInHeirarchy() const
        {
            if(!m_active)
                return false;
            if(m_parent)
                return m_parent->IsActiveInHeirarchy();
            return true;
        }

    public:
        void SetParent(const ReferenceView<GameObject>& gameObject);
        void SetActive(bool active);

    private:
        void RemoveComponent(const ReferenceView<Component>& component);
        void SetComponentActive(const ReferenceView<Component>& component, bool active);

    private:
        void AddChild(GameObject* gameObject);
        void RemoveChild(GameObject* gameObject);

        void SetWorldActive(bool active);
        void SetAllComponentsWorldActive(bool active);
        void SetChildrenWorldActive(bool active);
    };

    class GameObjectManager
    {
    protected:
        std::vector<ReferencePointer<GameObject>> m_managedComponents;

        std::priority_queue<size_t> m_destroyedComponentIndices;
        std::vector<std::pair<ReferenceView<GameObject>, bool>> m_activeChangedComponents;
    public:
        void ClearDestroyedComponents();
        void UpdateComponentStates()
        {
            if(m_activeChangedComponents.empty())
                return;

            for(const auto& component : m_activeChangedComponents)
            {
                if(!component.first)
                    continue;

                component.first->m_hasRequestedActiveChanged = false;

                if(!component.first->m_activeChanged)
                    continue;
                component.first->m_activeChanged = false;

                component.first->SetAllComponentsWorldActive(component.second);
                component.first->SetChildrenWorldActive(component.second);
            }

            m_activeChangedComponents.clear();
        }

        void DestroyComponent(const ReferenceView<Component>& component);
        void SetGameObjectActive(const ReferenceView<GameObject>& gameObject, bool active)
        {
            if(gameObject->m_isDestroyed)
                return;

            if(gameObject->m_activeChanged)
                gameObject->m_activeChanged = false;
            else
            {
                gameObject->m_activeChanged = true;

                if(!gameObject->m_hasRequestedActiveChanged)
                {
                    m_activeChangedComponents.push_back(std::pair(gameObject, active));
                    gameObject->m_hasRequestedActiveChanged = true;
                }
            }
        }
    };

    class Scene
    {
        friend class GameObject;

        GameObjectManager m_gameObjectManager;
        std::unordered_map<std::string, std::unique_ptr<ComponentManagerBase>> m_componentManagerMap;
        std::vector<ComponentManagerBase*> m_componentManagerVector;

        bool m_sceneLoaded = false;
        bool m_sceneDestroyed = false;

    public:
        ~Scene();

    public:
        bool IsSceneLoaded() const { return m_sceneLoaded; }

    private:
        ReferenceView<Component> CreateComponent(std::string type, const ReferenceView<GameObject>& gameObject);

        void DestroyComponent(const ReferenceView<Component>& component) { m_componentManagerMap[typeid(*component).name()]->DestroyComponent(component); }
        void SetComponentActive(const ReferenceView<Component>& component, bool active) { m_componentManagerMap[typeid(*component).name()]->SetComponentActive(component, active); }
        void SetComponentActiveWorld(const ReferenceView<Component>& component, bool active) {  m_componentManagerMap[typeid(*component).name()]->SetComponentActiveWorld(component, active); }

        void SetGameObjectActive(const ReferenceView<GameObject>& gameObject, bool active) { m_gameObjectManager.SetGameObjectActive(gameObject, active); }
    };
}