#pragma once
#include <Engine/Manager/SingletonManager/Singleton.h>

class EngineSystem : public Singleton<EngineSystem>
{
	friend class Singleton<EngineSystem>;
private:
	explicit EngineSystem();
public:
	virtual ~EngineSystem();
public:
	bool Initialize();
	void LateInitialize();
	void Update();
	void Render(HDC hdc);
	void Release();
};

