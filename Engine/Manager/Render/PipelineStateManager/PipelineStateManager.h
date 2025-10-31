#pragma once

class PipelineStateManager
{
public:
    PipelineStateManager();
    PipelineStateManager(const PipelineStateManager&);
    ~PipelineStateManager();

    // Device�� ���� ��ü�� �����ϰ�, Context�� ��� ����
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();

	// TODO - �پ��� ���� ��ü���� �����ϱ� ���� Get/Set �߰�
    // ID3D11RasterizerState* GetRasterizerState(ERasterizerStateType type);
    // void SetRasterizerState(ERasterizerStateType type);

private:
    // ���� ��ü��
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11RasterizerState* m_rasterState;

    // ���� ������ ���� ���ؽ�Ʈ (������ ����)
    ID3D11DeviceContext* m_deviceContext;
};