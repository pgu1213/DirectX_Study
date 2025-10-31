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
    // 포인터 복사
    m_device = device;
    m_deviceContext = context;
    m_swapChain = swapChain;
    m_vsync_enabled = vsync;

    HRESULT result;

    // 렌더 타겟 뷰 (RTV) 생성
    ID3D11Texture2D* backBufferPtr = nullptr;
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
    {
        return false;
    }

    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    if (FAILED(result))
    {
        backBufferPtr->Release(); // 실패 시에도 릴리즈
        return false;
    }

    // RTV가 백 버퍼에 대한 참조를 가졌으므로 포인터 해제
    backBufferPtr->Release();
    backBufferPtr = 0;

    // 깊이/스텐실 리소스 (DSV) 생성
    result = CreateDepthStencilResources(screenWidth, screenHeight);
    if (FAILED(result))
    {
        return false;
    }

    // 렌더 타겟 바인딩
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

    // m_device, m_deviceContext, m_swapChain은 GraphicsDeviceManager에서 처리
}

// 윈도우 크기 변경 처리를 위한 함수
bool SwapChainManager::OnResize(int newWidth, int newHeight)
{
    // 기존 RTV, DSV, 깊이 버퍼 해제
    if (m_renderTargetView) m_renderTargetView->Release();
    if (m_depthStencilView) m_depthStencilView->Release();
    if (m_depthStencilBuffer) m_depthStencilBuffer->Release();

    // 파이프라인에서 렌더 타겟 바인딩 해제
    m_deviceContext->OMSetRenderTargets(0, 0, 0);

    // 스왑 체인 버퍼 크기 변경
    HRESULT result = m_swapChain->ResizeBuffers(
        1,                          // 버퍼 개수
        newWidth,                   // 새 너비
        newHeight,                  // 새 높이
        DXGI_FORMAT_R8G8B8A8_UNORM, // 포맷
        0                           // 플래그
    );
    if (FAILED(result))
    {
        return false;
    }

    // RTV 재생성
    ID3D11Texture2D* backBufferPtr = nullptr;
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
    {
        return false;
    }

    result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    backBufferPtr->Release(); // RTV가 참조를 가져갔으므로 해제
    if (FAILED(result))
    {
        return false;
    }

    // 5. DSV 재생성
    result = CreateDepthStencilResources(newWidth, newHeight);
    if (FAILED(result))
    {
        return false;
    }

    // 6. 렌더 타겟 다시 바인딩
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    // 참고: 이 작업이 끝난 후, ViewportManager의 OnResize도 호출하여 
    // 뷰포트와 투영 행렬을 갱신 필요함

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

    // 백 버퍼(RTV)를 지정된 색상으로 클리어
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

    // 깊이/스텐실 버퍼(DSV) 클리어
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// EndScene
void SwapChainManager::Present()
{
    // VSync 설정에 따라 화면에 프레임을 표시
    m_swapChain->Present(m_vsync_enabled ? 1 : 0, 0);
}


// (Initialize와 OnResize에서 공통으로 사용하는 헬퍼 함수)
bool SwapChainManager::CreateDepthStencilResources(int width, int height)
{
    HRESULT result;

    // 깊이 버퍼 텍스처 생성
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1; // 멀티샘플링 안 함
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

    // 깊이 스텐실 뷰 생성
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