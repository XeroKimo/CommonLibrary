#pragma once
#include "IGameObject.h"

namespace CommonsLibrary
{
    template <class Type>
    ReferencePointer<Type> AddComponent(const ReferencePointer<IGameObject>& gameObject);

    template <class Type>
    ReferencePointer<Type> GetComponent(const ReferencePointer<IGameObject>& gameObject);

    template <class Type>
    std::vector<ReferencePointer<Type>> GetComponents(const ReferencePointer<IGameObject>& gameObject);
}

#include "GameObject.h"

namespace CommonsLibrary
{
    template <class Type>
    ReferencePointer<Type> AddComponent(const ReferencePointer<IGameObject>& gameObject)
    {
        return static_cast<GameObject*>(gameObject.Get())->AddComponent<Type>();
    }

    template <class Type>
    ReferencePointer<Type> GetComponent(const ReferencePointer<IGameObject>& gameObject)
    {
        return static_cast<GameObject*>(gameObject.Get())->GetComponent<Type>();
    }

    template <class Type>
    std::vector<ReferencePointer<Type>> GetComponents(const ReferencePointer<IGameObject>& gameObject)
    {
        return static_cast<GameObject*>(gameObject.Get())->GetComponents<Type>();
    }
}