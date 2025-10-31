#include <pch.h>
#include "PipelineStateManager.h"

PipelineStateManager::PipelineStateManager()
{
    m_depthStencilState = nullptr;
    m_rasterState = nullptr;
    m_deviceContext = nullptr;
}

PipelineStateManager::PipelineStateManager(const PipelineStateManager& other)
{
}

PipelineStateManager::~PipelineStateManager()
{
    Shutdown();
}

bool PipelineStateManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
    // 나중에 상태를 변경할 수 있도록 컨텍스트 포인터를 저장
    m_deviceContext = context;

    HRESULT result;

    // --- 1. 깊이/스텐실 상태 생성 (기존 코드 Block 5) ---
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // 픽셀이 앞면일 때 스텐실 연산
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 픽셀이 뒷면일 때 스텐실 연산
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 깊이 스텐실 상태를 생성
    result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
    if (FAILED(result))
    {
        return false;
    }

    // 생성한 깊이 스텐실 상태를 파이프라인에 즉시 설정
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


    // 래스터라이저 상태 생성
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));

    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK; // 뒷면 컬링
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID; // 픽셀을 채움
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // 래스터화 상태 변수를 생성
    result = device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if (FAILED(result))
    {
        return false;
    }

    // 생성한 래스터화 상태를 파이프라인에 즉시 설정
    m_deviceContext->RSSetState(m_rasterState);

    return true;
}

void PipelineStateManager::Shutdown()
{
    // 이 매니저가 소유한 COM 객체 해제
    if (m_rasterState)
    {
        m_rasterState->Release();
        m_rasterState = nullptr;
    }

    if (m_depthStencilState)
    {
        m_depthStencilState->Release();
        m_depthStencilState = nullptr;
    }
}