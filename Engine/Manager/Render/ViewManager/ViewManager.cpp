#include <pch.h>
#include "ViewManager.h"

ViewManager::ViewManager()
{
    m_deviceContext = nullptr;
    m_screenNear = 0.0f;
    m_screenDepth = 0.0f;
    // m_projectionMatrix, m_orthoMatrix�� XMMATRIXIdentity()�� �ڵ� �ʱ�ȭ��
    ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
}

ViewManager::~ViewManager()
{
}

bool ViewManager::Initialize(ID3D11DeviceContext* context, int screenWidth, int screenHeight, float screenNear, float screenDepth)
{
    m_deviceContext = context;
    m_screenNear = screenNear;     // OnResize�� ���� ����
    m_screenDepth = screenDepth;  // OnResize�� ���� ����

    // ����Ʈ ����
    m_viewport.Width = (float)screenWidth;
    m_viewport.Height = (float)screenHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

    // ����Ʈ�� ����������(Rasterizer Stage)�� ����
    m_deviceContext->RSSetViewports(1, &m_viewport);

    // ���� ��� ����
    // ���� ��� �ʱ�ȭ�� ���� (������Ʈ���� ���� ����)
    UpdateMatrices(screenWidth, screenHeight);

    return true;
}

// ������ ũ�Ⱑ ����� �� ȣ��
void ViewManager::OnResize(int newWidth, int newHeight)
{
    // ����Ʈ ũ�� ���� �� ���������ο� �缳��
    m_viewport.Width = (float)newWidth;
    m_viewport.Height = (float)newHeight;
    m_deviceContext->RSSetViewports(1, &m_viewport);

    // ���ŵ� ��Ⱦ��� ���� ��� �����
    UpdateMatrices(newWidth, newHeight);
}

// ��� ���� ���� �Լ�
void ViewManager::UpdateMatrices(int screenWidth, int screenHeight)
{
    // ��Ⱦ�� ��� (0���� ������ ����)
    float screenAspect = (float)screenWidth / (float)screenHeight;
    if (screenHeight == 0)
    {
        screenAspect = 1.0f;
    }

    // 3D �������� ���� ���� ��� (����)
    float fieldOfView = 3.141592654f / 4.0f; // 45��
    m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_screenNear, m_screenDepth);

    // 2D �������� ���� ���� ���
    m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, m_screenNear, m_screenDepth);
}