#pragma once
#include "Component.h"
#include "../World.h"

namespace CommonsLibrary
{
    template <class T, std::enable_if_t<std::is_base_of_v<ECSSystem, T>, int>>
    T* Component::GetSystem() const
    {
        return GetGameObject()->m_scene->m_world->GetSystemManager()->GetSystem<T>();
    }
}