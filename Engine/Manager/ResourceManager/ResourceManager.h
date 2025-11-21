#pragma once
#include <Engine/Manager/SingletonManager/Singleton.h>

class ResourceManager : public Singleton<ResourceManager>
{
	friend class Singleton<ResourceManager>;
private:
	explicit ResourceManager();
public:
	virtual ~ResourceManager();

public:
	void Initialize(string modelsPath);
	void Shutdown();
public:
	bool LoadModelData(string id, string filename);
	ModelData* GetModel(string id);

private:
	map<string, ModelData*> m_models;
};

