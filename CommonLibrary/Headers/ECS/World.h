#pragma once
#include "Component.h"
#include "ComponentRegistry.h"
#include "Scene.h"
#include "GameObject.h"

namespace CommonsLibrary
{
    class World : ReferenceFromThis<World>
    {
    private:
        //std::vector<std::unique_ptr<Scene>> m_scenes;
        //Scene* m_activeScene = nullptr;

    public:
        ReferencePointer<GameObject> Instantiate();
    };
}