#include "World.h"

namespace CommonsLibrary
{
    ReferencePointer<GameObject> World::Instantiate()
    {
        ReferencePointer<GameObject> instantitatedObject = MakeReference<GameObject>(GetReferencePointer());
        ReferencePointer<GameObject> returnObject = instantitatedObject;
        //m_activeScene->AddGameObject(std::move(instantitatedObject));
        return returnObject;
    }
}