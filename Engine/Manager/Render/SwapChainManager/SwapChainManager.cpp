#include <pch.h>
#include "SwapChainManager.h"

SwapChainManager::SwapChainManager()
{
    m_device = nullptr;
    m_deviceContext = nullptr;
    m_swapChain = nullptr;
    m_renderTargetView = nullptr;
    m_depthStencilBuffer = nullptr;
    m_depthStencilView = nullptr;
    m_vsync_enabled = false;
}

SwapChainManager::~SwapChainManager()
{
    Shutdown();
}

bool SwapChainManager::Initialize(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    IDXGISwapChain* swapChain,
    int screenWidth,
    int screenHeight,
    bool vsync)
{
    // ������ ����
    m_device = device;
    m_deviceContext = context;
    m_swapChain = swapChain;
    m_vsync_enabled = vsync;

    HRESULT result;

    // ���� Ÿ�� �� (RTV) ����
    ID3D11Texture2D* backBufferPtr = nullptr;
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
    {
        return false;
    }

    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    if (FAILED(result))
    {
        backBufferPtr->Release(); // ���� �ÿ��� ������
        return false;
    }

    // RTV�� �� ���ۿ� ���� ������ �������Ƿ� ������ ����
    backBufferPtr->Release();
    backBufferPtr = 0;

    // ����/���ٽ� ���ҽ� (DSV) ����
    result = CreateDepthStencilResources(screenWidth, screenHeight);
    if (FAILED(result))
    {
        return false;
    }

    // ���� Ÿ�� ���ε�
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    return true;
}

void SwapChainManager::Shutdown()
{
    if (m_renderTargetView)
    {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if (m_depthStencilView)
    {
        m_depthStencilView->Release();
        m_depthStencilView = nullptr;
    }

    if (m_depthStencilBuffer)
    {
        m_depthStencilBuffer->Release();
        m_depthStencilBuffer = nullptr;
    }

    // m_device, m_deviceContext, m_swapChain�� GraphicsDeviceManager���� ó��
}

// ������ ũ�� ���� ó���� ���� �Լ�
bool SwapChainManager::OnResize(int newWidth, int newHeight)
{
    // ���� RTV, DSV, ���� ���� ����
    if (m_renderTargetView) m_renderTargetView->Release();
    if (m_depthStencilView) m_depthStencilView->Release();
    if (m_depthStencilBuffer) m_depthStencilBuffer->Release();

    // ���������ο��� ���� Ÿ�� ���ε� ����
    m_deviceContext->OMSetRenderTargets(0, 0, 0);

    // ���� ü�� ���� ũ�� ����
    HRESULT result = m_swapChain->ResizeBuffers(
        1,                          // ���� ����
        newWidth,                   // �� �ʺ�
        newHeight,                  // �� ����
        DXGI_FORMAT_R8G8B8A8_UNORM, // ����
        0                           // �÷���
    );
    if (FAILED(result))
    {
        return false;
    }

    // RTV �����
    ID3D11Texture2D* backBufferPtr = nullptr;
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
    {
        return false;
    }

    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    backBufferPtr->Release(); // RTV�� ������ ���������Ƿ� ����
    if (FAILED(result))
    {
        return false;
    }

    // 5. DSV �����
    result = CreateDepthStencilResources(newWidth, newHeight);
    if (FAILED(result))
    {
        return false;
    }

    // 6. ���� Ÿ�� �ٽ� ���ε�
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // ����: �� �۾��� ���� ��, ViewportManager�� OnResize�� ȣ���Ͽ� 
    // ����Ʈ�� ���� ����� ���� �ʿ���

    return true;
}


// BeginScene
void SwapChainManager::ClearRenderTargets(float red, float green, float blue, float alpha)
{
    float color[4];
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;

    // �� ����(RTV)�� ������ �������� Ŭ����
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

    // ����/���ٽ� ����(DSV) Ŭ����
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// EndScene
void SwapChainManager::Present()
{
    // VSync ������ ���� ȭ�鿡 �������� ǥ��
    m_swapChain->Present(m_vsync_enabled ? 1 : 0, 0);
}


// (Initialize�� OnResize���� �������� ����ϴ� ���� �Լ�)
bool SwapChainManager::CreateDepthStencilResources(int width, int height)
{
    HRESULT result;

    // ���� ���� �ؽ�ó ����
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1; // ��Ƽ���ø� �� ��
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // ���� ���ٽ� �� ����
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}