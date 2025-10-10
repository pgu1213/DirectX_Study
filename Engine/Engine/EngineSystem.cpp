#include <pch.h>
#include "EngineSystem.h"
#include <Engine/Manager/TimeManager/TimeManager.h>
#include <Engine/Manager/InputManager/InputManager.h>

EngineSystem::EngineSystem()
{
}

EngineSystem::~EngineSystem()
{
}

bool EngineSystem::Initialize()
{
	printf("Engine Initialize..\n");

	return true;
}

void EngineSystem::LateInitialize()
{
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
