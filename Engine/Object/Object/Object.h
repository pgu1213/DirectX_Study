#pragma once
#include <Engine/Object/IPrototypeable/IPrototypeable.h>
#include <Engine/Object/Component/Component.h>

class Component;
class IPrototypeable;

class Object : public enable_shared_from_this<Object>
{
public:
    Object();
    Object(const Object& other);
    virtual ~Object();

    Object& operator=(const Object& other);

    virtual void Init();
    virtual void Update(float DeltaTime);
    virtual void LateUpdate(float DeltaTime);

    virtual void Render(ID3D11DeviceContext* context);

    unique_ptr<Object> Clone() const;

    void SetActive(bool bActive) { m_bisActive = bActive; }
    bool IsActive() const { return m_bisActive; }

    Transform& GetTransform() { return m_Transform; }
    const Transform& GetTransform() const { return m_Transform; }

    void SetParent(Object* parent);

    template <typename T>
    void AddComponent(unique_ptr<T> component)
    {
        component->SetOwner(this);
        m_ComponentMap[type_index(typeid(T))] = component.get();
        m_Components.push_back(move(component));
    }

private:
    void CloneComponents(const Object& other);
    void CloneChildren(const Object& other);

protected:
    string      m_Name;
    string      m_Tag;
    bool        m_bisActive = true;
    bool        m_bisStarted = false;

    Transform   m_Transform;
    Object* m_pParent = nullptr;

    vector<unique_ptr<Component>> m_Components;
    unordered_map<type_index, Component*> m_ComponentMap;

    vector<Object*> m_Children;

public:
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(is_base_of_v<Component, T>, "T는 Component로부터 상속받아야 합니다.");

        // 이미 존재하는 컴포넌트인지 확인
        if (HasComponent<T>())
            return GetComponent<T>();

        auto newComponent = make_unique<T>(this, forward<Args>(args)...);
        T* ptr = newComponent.get();

        m_Components.push_back(move(newComponent));
        m_ComponentMap[type_index(typeid(T))] = ptr;

        ptr->Init();
        return ptr;
    }

    template<typename T>
    T* GetComponent()
    {
        auto it = m_ComponentMap.find(type_index(typeid(T)));
        return (it != m_ComponentMap.end()) ? static_cast<T*>(it->second) : nullptr;
    }

    template<typename T>
    bool HasComponent()
    {
        return m_ComponentMap.find(type_index(typeid(T))) != m_ComponentMap.end();
    }

    template<typename T>
    void RemoveComponent()
    {
        auto it = m_ComponentMap.find(type_index(typeid(T)));
        if (it != m_ComponentMap.end())
        {
            Component* comp = it->second;
            comp->OnDestroy();

            m_Components.erase(
                remove_if(m_Components.begin(), m_Components.end(),
                    [comp](const unique_ptr<Component>& ptr) {
                        return ptr.get() == comp;
                    }),
                m_Components.end()
            );

            m_ComponentMap.erase(it);
        }
    }
};