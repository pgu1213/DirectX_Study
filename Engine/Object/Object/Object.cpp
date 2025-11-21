#include <pch.h>
#include "Object.h"

Object::Object()
{
}

Object::Object(const Object& other)
    : m_Name(other.m_Name + "_Clone")
    , m_Tag(other.m_Tag)
    , m_bisActive(other.m_bisActive)
    , m_Transform(other.m_Transform)
    , m_pParent(nullptr)
{
    CloneComponents(other);
    CloneChildren(other);
}

Object::~Object()
{
    for (auto* child : m_Children)
    {
        delete child;
    }
    m_Children.clear();
}

void Object::Init()
{
}

void Object::Update(float DeltaTime)
{
    if (!m_bisActive) return;

    if (!m_bisStarted)
    {
        for (auto& component : m_Components)
        {
            if (component->IsEnabled())
                component->Init();
        }
        m_bisStarted = true;
    }

    // Transform(월드 행렬) 계산
    // 부모가 있다면 부모의 월드 행렬을 기반으로 내 월드 행렬 계산
    XMMATRIX* parentMatrix = nullptr;
    if (m_pParent)
    {
        parentMatrix = &m_pParent->GetTransform().WorldMatrix;
    }
    m_Transform.UpdateTransform(parentMatrix);


    // 3. 컴포넌트 업데이트
    for (auto& component : m_Components)
    {
        if (component->IsEnabled())
            component->Update(DeltaTime);
    }

    // 4. 자식 오브젝트 업데이트
    for (auto* child : m_Children)
    {
        child->Update(DeltaTime);
    }
}

void Object::LateUpdate(float DeltaTime)
{
    if (!m_bisActive) return;

    for (auto& component : m_Components)
    {
        if (component->IsEnabled())
            component->LateUpdate(DeltaTime);
    }

    for (auto* child : m_Children)
    {
        child->LateUpdate(DeltaTime);
    }
}

void Object::Render(ID3D11DeviceContext* context)
{
    if (!m_bisActive) return;

    // 컴포넌트가 있다면 여기서 Transform의 WorldMatrix를 셰이더 상수 버퍼로 보낸 뒤 DrawCall을 수행
    for (auto& component : m_Components)
    {
        if (component->IsEnabled())
            component->Render(context);
    }

    // 자식 렌더링
    for (auto* child : m_Children)
    {
        child->Render(context);
    }
}

unique_ptr<Object> Object::Clone() const
{
    return make_unique<Object>(*this);
}

Object& Object::operator=(const Object& other)
{
    if (this != &other)
    {
        m_Name = other.m_Name + "_Clone";
        m_Tag = other.m_Tag;
        m_bisActive = other.m_bisActive;
        m_Transform = other.m_Transform;
        m_pParent = nullptr;

        m_Components.clear();
        m_ComponentMap.clear();

        for (auto* child : m_Children)
        {
            delete child;
        }
        m_Children.clear();

        CloneComponents(other);
        CloneChildren(other);
    }
    return *this;
}

void Object::CloneComponents(const Object& other)
{
    for (const auto& component : other.m_Components)
    {
        auto clonedComponent = component->Clone();
        Component* ptr = static_cast<Component*>(clonedComponent.get());
        ptr->SetOwner(this);

        m_Components.push_back(unique_ptr<Component>(ptr));
        m_ComponentMap[type_index(typeid(*ptr))] = ptr;
        clonedComponent.release();
    }
}

void Object::CloneChildren(const Object& other)
{
    for (const Object* child : other.m_Children)
    {
        unique_ptr<Object> clonedChild = child->Clone();
        clonedChild->SetParent(this);

        m_Children.push_back(clonedChild.release());
    }
}

void Object::SetParent(Object* parent)
{
    m_pParent = parent;
}