#pragma once

class PipelineStateManager
{
public:
    PipelineStateManager();
    PipelineStateManager(const PipelineStateManager&);
    ~PipelineStateManager();

    // Device로 상태 객체를 생성하고, Context에 즉시 설정
    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Shutdown();

	// TODO - 다양한 상태 객체들을 관리하기 위한 Get/Set 추가
    // ID3D11RasterizerState* GetRasterizerState(ERasterizerStateType type);
    // void SetRasterizerState(ERasterizerStateType type);

private:
    // 상태 객체들
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11RasterizerState* m_rasterState;

    // 상태 설정을 위한 컨텍스트 (소유권 없음)
    ID3D11DeviceContext* m_deviceContext;
};