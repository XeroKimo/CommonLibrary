//#pragma once
//#include "GameObject.h"
//
//namespace CommonsLibrary
//{
//    class Scene
//    {
//    private:
//        std::vector<ReferencePointer<GameObject>> m_activeGameObjects;
//        std::vector<ReferencePointer<GameObject>> m_inactiveGameObjects;
//
//    public:
//        Scene() = default;
//
//    public:
//        void Update(float deltaTime);
//
//        virtual void LoadScene() = 0;
//
//    public:
//        void AddGameObject(ReferencePointer<GameObject> gameObject);
//        void DeleteGameObject(const ReferencePointer<GameObject>& gameObject);
//        ReferencePointer<GameObject> FindObject(const std::string& name);
//
//        template <class Type>
//        ReferencePointer<Type> FindObjectType()
//        {
//            ReferencePointer<Type> component = FindObjectType<Type>();
//            return (component) ? component : FindObjectType<Type>();
//        }
//        template <class Type>
//        std::vector<ReferencePointer<Type>> FindObjectsType()
//        {
//            std::vector<ReferencePointer<Type>> components;
//            FindObjectsType<Type>(m_activeGameObjects, components);
//            FindObjectsType<Type>(m_inactiveGameObjects, components);
//            return components;
//        }
//
//    private:
//        bool FindObjectToDelete(std::vector<ReferencePointer<GameObject>>& objectVector, const ReferencePointer<GameObject>& gameObject);
//        ReferencePointer<GameObject> FindObject(const std::vector<ReferencePointer<GameObject>>& objectVector, const std::string& name);
//
//        template <class Type>
//        ReferencePointer<Type> FindObjectType(const std::vector<ReferencePointer<GameObject>>& objectVector)
//        {
//            for (const ReferencePointer<GameObject>& gameObject : objectVector)
//            {
//                ReferencePointer<Type> component = gameObject->GetComponent<Type>();
//                if (component)
//                    return component;
//            }
//            return nullptr;
//        }
//
//        template <class Type>
//        void FindObjectsType(const std::vector<ReferencePointer<GameObject>>& objectVector, std::vector<ReferencePointer<Type>>& outputVector)
//        {
//            for (const ReferencePointer<GameObject>& gameObject : objectVector)
//            {
//                ReferencePointer<Type> component = gameObject->GetComponent<Type>();
//                if (component)
//                    return component;
//            }
//            return nullptr;
//        }
//    };
//}