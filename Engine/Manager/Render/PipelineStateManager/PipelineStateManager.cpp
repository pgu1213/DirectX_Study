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
    // ���߿� ���¸� ������ �� �ֵ��� ���ؽ�Ʈ �����͸� ����
    m_deviceContext = context;

    HRESULT result;

    // --- 1. ����/���ٽ� ���� ���� (���� �ڵ� Block 5) ---
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // �ȼ��� �ո��� �� ���ٽ� ����
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // �ȼ��� �޸��� �� ���ٽ� ����
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // ���� ���ٽ� ���¸� ����
    result = device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
    if (FAILED(result))
    {
        return false;
    }

    // ������ ���� ���ٽ� ���¸� ���������ο� ��� ����
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


    // �����Ͷ����� ���� ����
    D3D11_RASTERIZER_DESC rasterDesc;
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));

    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK; // �޸� �ø�
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID; // �ȼ��� ä��
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // ������ȭ ���� ������ ����
    result = device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if (FAILED(result))
    {
        return false;
    }

    // ������ ������ȭ ���¸� ���������ο� ��� ����
    m_deviceContext->RSSetState(m_rasterState);

    return true;
}

void PipelineStateManager::Shutdown()
{
    // �� �Ŵ����� ������ COM ��ü ����
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