#pragma once
#include <Engine/Manager/SingletonManager/Singleton.h>

class InputManager : public Singleton<InputManager>
{
    friend class Singleton<InputManager>;
private:
    explicit InputManager();
public:
    virtual ~InputManager();
public:
    void Init();
    void Update();
    bool IsKeyPressed(int key);
    bool IsKeyDown(int key);
    bool IsKeyUp(int key);
    POINT GetMousePosition();
private:
    bool m_KeyStates[256];
    bool m_PrevKeyStates[256];
    POINT m_MousePos;
};

