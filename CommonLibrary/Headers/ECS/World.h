#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "GameObject.h"
#include "Scene.h"
#include <memory>

namespace CommonsLibrary
{
    class Scene;
    class World : ReferenceFromThis<World>
    {
    private:
        std::vector<std::unique_ptr<Scene>> m_scenes;
        Scene* m_activeScene = nullptr;

    public:
        ReferencePointer<GameObject> Instantiate();
    };
}