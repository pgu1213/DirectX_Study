#include <pch.h>
#include "GraphicsManager.h"
#include <Engine/Manager/Render/SwapChainManager/SwapChainManager.h>
#include <Engine/Manager/Render/PipelineStateManager/PipelineStateManager.h>
#include <Engine/Manager/Render/ViewManager/ViewManager.h>

GraphicsManager::GraphicsManager()
{
    m_device = nullptr;
    m_deviceContext = nullptr;
    m_swapChain = nullptr;
    m_BIsVsync = false;
    m_videoCardMemory = 0;
    m_videoCardDescription[0] = '\0';
}

GraphicsManager::~GraphicsManager()
{
    Shutdown();
}

bool GraphicsManager::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen)
{
    HRESULT result;
    IDXGIFactory* factory;
    IDXGIAdapter* adapter;
    IDXGIOutput* adapterOutput;
    unsigned int numModes, numerator = 0, denominator = 1;
    unsigned long long stringLength;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    int error;
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    D3D_FEATURE_LEVEL featureLevel;

    // vsync 설정
    m_BIsVsync = vsync;

    // 어댑터 및 모니터 정보 탐색
    // DirectX 그래픽 인터페이스 팩토리를 생성
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
    {
        return false;
    }

    // 기본 그래픽 카드 어댑터 생성
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
    {
        factory->Release();
        return false;
    }

    // 모니터(어댑터 출력)를 나열
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result))
    {
        adapter->Release();
        factory->Release();
        return false;
    }

    // 디스플레이 모드 개수를 가져옴
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if (FAILED(result))
    {
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // 디스플레이 모드 리스트를 생성
    displayModeList = new DXGI_MODE_DESC[numModes];
    if (!displayModeList)
    {
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // 디스플레이 모드 리스트를 채움
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result))
    {
        delete[] displayModeList;
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // 화면 너비/높이에 맞는 주사율(numerator/denominator)을 찾음
    for (unsigned int i = 0; i < numModes; i++)
    {
        if (displayModeList[i].Width == (unsigned int)screenWidth)
        {
            if (displayModeList[i].Height == (unsigned int)screenHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    // 그래픽 카드 어댑터 description을 획득
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
    {
        delete[] displayModeList;
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // 그래픽 카드 메모리(MB)와 이름을 저장
    m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
    error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
    {
        // 실패해도 치명적이진 않음
    }

    // 사용한 객체 해제
    delete[] displayModeList;
    displayModeList = 0;

    adapterOutput->Release();
    adapterOutput = 0;

    adapter->Release();
    adapter = 0;

    factory->Release();
    factory = 0;

    // Device, Context, SwapChain 생성

    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (m_BIsVsync)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = !fullscreen;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    featureLevel = D3D_FEATURE_LEVEL_11_0;

    // 스왑 체인, D3D 장치, D3D 장치 컨텍스트를 생성
    // 결과물을 이 클래스의 멤버 변수(m_swapChain, m_device, m_deviceContext)에 저장
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

    if (FAILED(result))
    {
        return false;
    }

	// 서브 매니저 초기화
    if (!SwapChainManager::GetInstance()->Initialize(m_device, m_deviceContext, m_swapChain, WINCX ,WINCY, false))
    {
		printf("Failed to initialize SwapChainManager.\n");
		return false;
    }

    if (!PipelineStateManager::GetInstance()->Initialize(m_device, m_deviceContext))
    {
		printf("Failed to initialize PipelineStateManager.\n");
		return false;
    }

    if (!ViewManager::GetInstance()->Initialize(m_deviceContext, WINCX, WINCY, 10, 100))
    {
		printf("Failed to initialize ViewManager.\n");
		return false;
    }

    return true;
}

void GraphicsManager::Shutdown()
{
    // 종료 시 전체 화면 모드라면 창 모드로 전환 (오류 방지용)
    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(false, NULL);
    }

    // COM 객체들을 안전하게 해제
    if (m_swapChain)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }

    if (m_deviceContext)
    {
        m_deviceContext->Release();
        m_deviceContext = nullptr;
    }

    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }
}

void GraphicsManager::GetVideoCardInfo(char* cardName, int& memory)
{
    strcpy_s(cardName, 128, m_videoCardDescription);
    memory = m_videoCardMemory;
}