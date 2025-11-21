#include <pch.h>
#include "EngineSystem.h"
#include <Engine/Manager/TimeManager/TimeManager.h>
#include <Engine/Manager/InputManager/InputManager.h>
#include <Engine/Manager/Render/GraphicsManager/GraphicsManager.h>
#include <Engine/Manager/Render/SwapChainManager/SwapChainManager.h>
#include <Engine/Manager/Render/PipelineStateManager/PipelineStateManager.h>
#include <Engine/Manager/Render/ViewManager/ViewManager.h>
#include <Engine/Manager/ResourceManager/ResourceManager.h>

EngineSystem::EngineSystem()
{
}

EngineSystem::~EngineSystem()
{
}

bool EngineSystem::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	printf("Engine Initialize..\n");

	GraphicsManager::GetInstance()->Initialize(screenWidth, screenHeight, false, hwnd, false);

	return true;
}

void EngineSystem::LateInitialize()
{
	ResourceManager::GetInstance()->Initialize("../Resource/Model");
}

void EngineSystem::Update()
{
	TimeManager::GetInstance()->Update();

	float DeltaTime = TimeManager::GetInstance()->GetDeltaTime();

	InputManager::GetInstance()->Update();

	// MainManager::GetInstance()->Update(DeltaTime); // 실제 매니저 업데이트
}

void EngineSystem::Render(HDC hdc)
{
}

void EngineSystem::Release()
{
}
