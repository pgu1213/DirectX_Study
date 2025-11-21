#pragma once
#include <Engine/Manager/SingletonManager/Singleton.h>

class SwapChainManager : public Singleton<SwapChainManager>
{
	friend class Singleton<SwapChainManager>;
private:
    explicit SwapChainManager();
    explicit SwapChainManager(const SwapChainManager&);

public:
    virtual ~SwapChainManager();

    // GraphicsDeviceManager로부터 핵심 객체들을 초기화
    bool Initialize(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        IDXGISwapChain* swapChain,
        int screenWidth,
        int screenHeight,
        bool vsync
    );

    void Shutdown();

    // 윈도우 크기 변경 시 호출
    bool OnResize(int newWidth, int newHeight);

    // 렌더링 시작 시 렌더 타겟과 깊이 버퍼 클리어
    void ClearRenderTargets(float red, float green, float blue, float alpha);

    // 렌더링 완료 후 스왑 체인을 화면에 표시
    void Present();

    // 외부(EX) 렌더러)에서 현재 RTV와 DSV를 가져갈 수 있도록 Get을 제공
    ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView; }
    ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView; }

private:
    // 깊이/스텐실 버퍼와 뷰를 생성하는 내부 헬퍼 함수입니다.
    // (Initialize와 OnResize에서 중복 사용됨)
    bool CreateDepthStencilResources(int width, int height);

private:
    // GraphicsDeviceManager로부터 받은 포인터 (소유권 없음)
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    IDXGISwapChain* m_swapChain;

    // 이 매니저가 직접 생성하고 소유하는 리소스
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;      // DSV를 만들기 위한 텍스처
    ID3D11DepthStencilView* m_depthStencilView;

    // Present 시 사용할 VSync 설정
    bool m_vsync_enabled;
};