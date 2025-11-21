#include <pch.h>
#include "ViewManager.h"

ViewManager::ViewManager()
{
    m_deviceContext = nullptr;
    m_screenNear = 0.0f;
    m_screenDepth = 0.0f;
    // m_projectionMatrix, m_orthoMatrix는 XMMATRIXIdentity()로 자동 초기화
    ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
}

ViewManager::~ViewManager()
{
}

bool ViewManager::Initialize(ID3D11DeviceContext* context, int screenWidth, int screenHeight, float screenNear, float screenDepth)
{
    m_deviceContext = context;
    m_screenNear = screenNear;     // OnResize를 위해 저장
    m_screenDepth = screenDepth;  // OnResize를 위해 저장

    // 뷰포트 설정
    m_viewport.Width = (float)screenWidth;
    m_viewport.Height = (float)screenHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    // 뷰포트를 파이프라인(Rasterizer Stage)에 설정
    m_deviceContext->RSSetViewports(1, &m_viewport);

    // 투영 행렬 생성
    // 월드 행렬 초기화는 제외 (오브젝트에서 구현 예정)
    UpdateMatrices(screenWidth, screenHeight);

    return true;
}

// 윈도우 크기가 변경될 때 호출
void ViewManager::OnResize(int newWidth, int newHeight)
{
    // 뷰포트 크기 갱신 및 파이프라인에 재설정
    m_viewport.Width = (float)newWidth;
    m_viewport.Height = (float)newHeight;
    m_deviceContext->RSSetViewports(1, &m_viewport);

    // 갱신된 종횡비로 투영 행렬 재생성
    UpdateMatrices(newWidth, newHeight);
}

// 행렬 생성 헬퍼 함수
void ViewManager::UpdateMatrices(int screenWidth, int screenHeight)
{
    // 종횡비 계산 (0으로 나누기 방지)
    float screenAspect = (float)screenWidth / (float)screenHeight;
    if (screenHeight == 0)
    {
        screenAspect = 1.0f;
    }

    // 3D 렌더링을 위한 투영 행렬 (원근)
    float fieldOfView = 3.141592654f / 4.0f; // 45도
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_screenNear, m_screenDepth);

    // 2D 렌더링을 위한 직교 행렬
    m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, m_screenNear, m_screenDepth);
}