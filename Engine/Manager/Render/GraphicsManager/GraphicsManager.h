#pragma once
#include <Engine/Manager/SingletonManager/Singleton.h>

class GraphicsManager : public Singleton<GraphicsManager>
{
    friend class Singleton<GraphicsManager>;

private:
    explicit GraphicsManager();

public:
    virtual ~GraphicsManager();

public:
    bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen);
    void Shutdown();

    // 생성된 핵심 객체들을 반환하는 함수
    ID3D11Device* GetDevice() const { return m_device; }
    ID3D11DeviceContext* GetContext() const { return m_deviceContext; }
    IDXGISwapChain* GetSwapChain() const { return m_swapChain; }

    // 탐색한 비디오 카드 정보를 반환하는 함수
    void GetVideoCardInfo(char* cardName, int& memory);

private:
    bool m_BIsVsync;
    int m_videoCardMemory;
    char m_videoCardDescription[128];

    // D3D11 인터페이스 포인터
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    IDXGISwapChain* m_swapChain;
};

