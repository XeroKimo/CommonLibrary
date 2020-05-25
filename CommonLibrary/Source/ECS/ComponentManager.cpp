#include "CommonsLibrary/ECS/ECS.h"
#include <algorithm>

namespace CommonsLibrary
{
    void ComponentManagerBase::ClearDestroyedComponents()
    {
        while(!m_destroyedComponentIndices.empty())
        {
            size_t index = m_destroyedComponentIndices.top();
            if(index < m_firstInactiveComponent)
            {
                m_firstInactiveComponent--;
                SwapComponents(index, m_firstInactiveComponent);
                index = m_firstInactiveComponent;
            }
            SwapComponents(index, m_managedComponents.size() - 1);

            m_managedComponents.pop_back();
            m_destroyedComponentIndices.pop();
        }
    }

    void ComponentManagerBase::DestroyComponent(const ReferenceView<Component>& component)
    {
        if(component->m_isDestroyed)
            return;

        m_destroyedComponentIndices.push(component->m_componentIndex);
        component->m_isDestroyed = true;
    }

    void ComponentManagerBase::SetComponentActive(const ReferenceView<Component>& component, bool active)
    {
        if(component->m_isDestroyed)
            return;
        if(component->m_active == active)
            return;

        component->m_active = active;

        if(component->m_gameObjectActiveWorld)
        {
            if(!component->m_hasRequestedActiveChanged)
            {
                m_activeChangedComponents.push_back(component);
                component->m_hasRequestedActiveChanged = true;
            }
        }
    }

    void ComponentManagerBase::SetComponentActiveWorld(const ReferenceView<Component>& component, bool active)
    {
        if(component->m_isDestroyed)
            return;
        if(component->m_gameObjectActiveWorld == active)
            return;

        component->m_gameObjectActiveWorld = active;

        if(!component->m_active)
            return;

        if(active)
        {
            if(!component->m_hasStarted)
                component->Start();
            SwapComponents(m_firstInactiveComponent, component->m_componentIndex);
            m_firstInactiveComponent++;
        }
        else
        {
            m_firstInactiveComponent--;
            SwapComponents(m_firstInactiveComponent, component->m_componentIndex);
        }
    }

    void ComponentManagerBase::SwapComponents(size_t lh, size_t rh)
    {
        if(lh == rh)
            return;

        ReferencePointer<Component> temp = std::move(m_managedComponents[lh]);
        m_managedComponents[lh] = std::move(m_managedComponents[rh]);
        m_managedComponents[rh] = std::move(temp);

        m_managedComponents[lh]->m_componentIndex = lh;
        m_managedComponents[rh]->m_componentIndex = rh;
    }
}