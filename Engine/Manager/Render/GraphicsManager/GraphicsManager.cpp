#include <pch.h>
#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
    m_device = nullptr;
    m_deviceContext = nullptr;
    m_swapChain = nullptr;
    m_vsync_enabled = false;
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

    // vsync ����
    m_vsync_enabled = vsync;

    // ����� �� ����� ���� Ž��
    // DirectX �׷��� �������̽� ���丮�� ����
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
    {
        return false;
    }

    // �⺻ �׷��� ī�� ����� ����
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
    {
        factory->Release();
        return false;
    }

    // �����(����� ���)�� ����
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result))
    {
        adapter->Release();
        factory->Release();
        return false;
    }

    // ���÷��� ��� ������ ������
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if (FAILED(result))
    {
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // ���÷��� ��� ����Ʈ�� ����
    displayModeList = new DXGI_MODE_DESC[numModes];
    if (!displayModeList)
    {
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // ���÷��� ��� ����Ʈ�� ä��
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result))
    {
        delete[] displayModeList;
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // ȭ�� �ʺ�/���̿� �´� �ֻ���(numerator/denominator)�� ã��
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

    // �׷��� ī�� ����� description�� ȹ��
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
    {
        delete[] displayModeList;
        adapterOutput->Release();
        adapter->Release();
        factory->Release();
        return false;
    }

    // �׷��� ī�� �޸�(MB)�� �̸��� ����
    m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
    error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
    {
        // �����ص� ġ�������� ����
    }

    // ����� ��ü ����
    delete[] displayModeList;
    displayModeList = 0;

    adapterOutput->Release();
    adapterOutput = 0;

    adapter->Release();
    adapter = 0;

    factory->Release();
    factory = 0;

    // Device, Context, SwapChain ����

    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (m_vsync_enabled)
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

    // ���� ü��, D3D ��ġ, D3D ��ġ ���ؽ�Ʈ�� ����
    // ������� �� Ŭ������ ��� ����(m_swapChain, m_device, m_deviceContext)�� ����
    result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

    if (FAILED(result))
    {
        return false;
    }

    // �� �Ŵ����� ������ ��������Դϴ�.
    // RTV, DSV, State, Viewport ���� �ٸ� �Ŵ����� �̾ ó���մϴ�.

    return true;
}

void GraphicsManager::Shutdown()
{
    // ���� �� ��ü ȭ�� ����� â ���� ��ȯ (���� ������)
    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(false, NULL);
    }

    // COM ��ü���� �����ϰ� ����
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