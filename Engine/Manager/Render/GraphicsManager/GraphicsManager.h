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

    // ������ �ٽ� ��ü���� ��ȯ�ϴ� �Լ�
    ID3D11Device* GetDevice() const { return m_device; }
    ID3D11DeviceContext* GetContext() const { return m_deviceContext; }
    IDXGISwapChain* GetSwapChain() const { return m_swapChain; }

    // Ž���� ���� ī�� ������ ��ȯ�ϴ� �Լ�
    void GetVideoCardInfo(char* cardName, int& memory);

private:
    bool m_vsync_enabled;
    int m_videoCardMemory;
    char m_videoCardDescription[128];

    // D3D11 �������̽� ������
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    IDXGISwapChain* m_swapChain;
};

