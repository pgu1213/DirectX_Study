#pragma once

class ViewManager
{
public:
    ViewManager();
    ~ViewManager();

    // 뷰포트를 설정하고 투영 행렬들을 생성합니다.
    bool Initialize(ID3D11DeviceContext* context, int screenWidth, int screenHeight, float screenNear, float screenDepth);

    // 윈도우 크기 변경 시 뷰포트와 투영 행렬을 갱신합니다.
    void OnResize(int newWidth, int newHeight);

    // 카메라 컴포넌트나 렌더러가 사용할 수 있도록 행렬을 반환합니다.
    const XMMATRIX& GetProjectionMatrix() const { return m_projectionMatrix; }
    const XMMATRIX& GetOrthoMatrix() const { return m_orthoMatrix; }
    const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

private:
    // Initialize와 OnResize에서 공통으로 사용할 행렬 업데이트 함수
    void UpdateMatrices(int screenWidth, int screenHeight);

private:
    // 뷰포트 설정을 위한 컨텍스트 (소유권 없음)
    ID3D11DeviceContext* m_deviceContext;

    D3D11_VIEWPORT m_viewport;
    XMMATRIX m_projectionMatrix; // 3D 렌더링용
    XMMATRIX m_orthoMatrix;      // 2D UI 렌더링용

    // OnResize 시에도 사용해야 하므로 클립 평면 값을 저장
    float m_screenNear;
    float m_screenDepth;
};