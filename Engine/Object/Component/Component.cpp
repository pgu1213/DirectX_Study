#include <pch.h>
#include "Component.h"
#include <Engine/Object/Object/Object.h> 
#include <Engine/Object/UI/UI.h>

Component::Component(Object* owner)
    : m_pObjOwner(owner)
    , m_pUIOwner(nullptr)
    , m_bisEnabled(true)
{
}

Component::Component(UI* owner)
    : m_pObjOwner(nullptr)
    , m_pUIOwner(owner)
    , m_bisEnabled(true)
{
}

// 프로토타입 클론
unique_ptr<IPrototypeable> Component::Clone() const
{
    // CloneImpl을 호출하여 unique_ptr로 감싸서 반환
    return unique_ptr<Component>(CloneImpl());
}

void Component::CopyFrom(const IPrototypeable* source)
{
    const Component* sourceComp = dynamic_cast<const Component*>(source);
    if (sourceComp)
    {
        m_bisEnabled = sourceComp->m_bisEnabled;
        // 주의: m_pObjOwner나 m_pUIOwner는 복사 X
    }
}

void Component::SetOwner(Object* newOwner)
{
    m_pObjOwner = newOwner;
    m_pUIOwner = nullptr;
}

void Component::SetOwner(UI* newOwner)
{
    m_pUIOwner = newOwner;
    m_pObjOwner = nullptr;
}

void Component::Init()
{
}

void Component::Update(float deltaTime)
{
}

void Component::LateUpdate(float deltaTime)
{
}

void Component::Render(ID3D11DeviceContext* context)
{
}

void Component::OnDestroy()
{
}

void Component::OnEnable()
{
}

void Component::OnDisable()
{
}