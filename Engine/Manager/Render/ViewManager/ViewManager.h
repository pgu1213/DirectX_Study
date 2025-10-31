#pragma once

class ViewManager
{
public:
    ViewManager();
    ~ViewManager();

    // ����Ʈ�� �����ϰ� ���� ��ĵ��� �����մϴ�.
    bool Initialize(ID3D11DeviceContext* context, int screenWidth, int screenHeight, float screenNear, float screenDepth);

    // ������ ũ�� ���� �� ����Ʈ�� ���� ����� �����մϴ�.
    void OnResize(int newWidth, int newHeight);

    // ī�޶� ������Ʈ�� �������� ����� �� �ֵ��� ����� ��ȯ�մϴ�.
    const XMMATRIX& GetProjectionMatrix() const { return m_projectionMatrix; }
    const XMMATRIX& GetOrthoMatrix() const { return m_orthoMatrix; }
    const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

private:
    // Initialize�� OnResize���� �������� ����� ��� ������Ʈ �Լ�
    void UpdateMatrices(int screenWidth, int screenHeight);

private:
    // ����Ʈ ������ ���� ���ؽ�Ʈ (������ ����)
    ID3D11DeviceContext* m_deviceContext;

    D3D11_VIEWPORT m_viewport;
    XMMATRIX m_projectionMatrix; // 3D ��������
    XMMATRIX m_orthoMatrix;      // 2D UI ��������

    // OnResize �ÿ��� ����ؾ� �ϹǷ� Ŭ�� ��� ���� ����
    float m_screenNear;
    float m_screenDepth;
};