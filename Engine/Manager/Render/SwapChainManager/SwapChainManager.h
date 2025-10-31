#pragma once

class SwapChainManager
{
public:
    SwapChainManager();
    SwapChainManager(const SwapChainManager&);
    ~SwapChainManager();

    // GraphicsDeviceManager�κ��� �ٽ� ��ü���� �ʱ�ȭ
    bool Initialize(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        IDXGISwapChain* swapChain,
        int screenWidth,
        int screenHeight,
        bool vsync
    );

    // ������ COM ��ü���� ����
    void Shutdown();

    // ������ ũ�� ���� �� ȣ��� �Լ�
    bool OnResize(int newWidth, int newHeight);

    // ������ ���� �� ���� Ÿ�ٰ� ���� ���� Ŭ����
    void ClearRenderTargets(float red, float green, float blue, float alpha);

    // ������ �Ϸ� �� ���� ü���� ȭ�鿡 ǥ��
    void Present();

    // �ܺ�(EX) ������)���� ���� RTV�� DSV�� ������ �� �ֵ��� Get�� ����
    ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView; }
    ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView; }

private:
    // ����/���ٽ� ���ۿ� �並 �����ϴ� ���� ���� �Լ��Դϴ�.
    // (Initialize�� OnResize���� �ߺ� ����)
    bool CreateDepthStencilResources(int width, int height);

private:
    // GraphicsDeviceManager�κ��� ���� ������ (������ ����)
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    IDXGISwapChain* m_swapChain;

    // �� �Ŵ����� ���� �����ϰ� �����ϴ� ���ҽ�
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;      // DSV�� ����� ���� �ؽ�ó
    ID3D11DepthStencilView* m_depthStencilView;

    // Present �� ����� VSync ����
    bool m_vsync_enabled;
};