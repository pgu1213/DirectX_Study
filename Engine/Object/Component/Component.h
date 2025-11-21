#pragma once
#include <Engine/Object/IPrototypeable/IPrototypeable.h>
#include <Engine/Object/Object/Object.h>
#include <Engine/Object/UI/UI.h>

class Object;
class UI;

class Component : public IPrototypeable
{
public:
    Component(Object* owner);
    Component(UI* owner);
    virtual ~Component() = default;

public:
    virtual Component* CloneImpl() const = 0;

    virtual void Init();                            // 컴포넌트 생성/시작 시
    virtual void Update(float deltaTime);           // 매 프레임 로직 업데이트
    virtual void LateUpdate(float deltaTime);       // 업데이트 후처리

    virtual void Render(ID3D11DeviceContext* context);

    virtual void OnDestroy();                       // 파괴 시
    virtual void OnEnable();                        // 활성화 시
    virtual void OnDisable();                       // 비활성화 시

public:
    unique_ptr<IPrototypeable> Clone() const override;
    void CopyFrom(const IPrototypeable* source) override;

    // Setter
    void SetOwner(Object* newOwner);
    void SetOwner(UI* newOwner);

    // Getter
    bool IsEnabled() const { return m_bisEnabled; }
    Object* GetObjOwner() const { return m_pObjOwner; }
    UI* GetUIOwner() const { return m_pUIOwner; }

protected:
    Object* m_pObjOwner;
    UI* m_pUIOwner;
    bool    m_bisEnabled;
};