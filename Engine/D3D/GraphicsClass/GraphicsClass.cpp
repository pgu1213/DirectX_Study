#include "GraphicsClass.h"
#include <Engine/D3D/D3DClass/D3DClass.h>

GraphicsClass::GraphicsClass()
{
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int, int, HWND)
{
    bool result;


    // Direct3D ��ü�� �����մϴ�.
    m_D3D = new D3DClass;
    if (!m_D3D)
    {
        return false;
    }

    // Direct3D ��ü�� �ʱ�ȭ�մϴ�.
    result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::Shutdown()
{
    // D3D ��ü�� ��ȯ�մϴ�.
    if (m_D3D)
    {
        m_D3D->Shutdown();
        delete m_D3D;
        m_D3D = 0;
    }

    return;
}

bool GraphicsClass::Update()
{
	return false;
}

bool GraphicsClass::Render()
{
    // �� �׸��⸦ �����ϱ� ���� ������ ������ ����ϴ�.
    m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);


    // ���ۿ� �׷��� ���� ȭ�鿡 ǥ���մϴ�.
    m_D3D->EndScene();

    return true;
}
