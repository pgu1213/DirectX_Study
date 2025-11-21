#pragma once

struct Transform
{
	XMFLOAT3 LocalPosition = { 0, 0, 0 };
	XMFLOAT3 LocalRotation = { 0, 0, 0 }; // Euler Angles (Degree or Radian)
	XMFLOAT3 LocalScale = { 1, 1, 1 };

	XMMATRIX WorldMatrix = XMMatrixIdentity(); // 최종 월드 행렬

	// 행렬 갱신 헬퍼
	void UpdateTransform(const XMMATRIX* parentMatrix = nullptr)
	{
		XMMATRIX S = XMMatrixScaling(LocalScale.x, LocalScale.y, LocalScale.z);
		XMMATRIX R = XMMatrixRotationRollPitchYaw(LocalRotation.x, LocalRotation.y, LocalRotation.z);
		XMMATRIX T = XMMatrixTranslation(LocalPosition.x, LocalPosition.y, LocalPosition.z);

		XMMATRIX localWorld = S * R * T;

		if (parentMatrix)
		{
			WorldMatrix = localWorld * (*parentMatrix);
		}
		else
		{
			WorldMatrix = localWorld;
		}
	}
};

/// <summary>
/// 모델의 데이터를 담는 정점 구조체
/// </summary>
struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
};

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

struct ModelData
{
	vector<ModelType> vertices;
};
