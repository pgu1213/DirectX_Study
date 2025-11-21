#include <pch.h>
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Initialize(string modelsPath)
{
    if (!filesystem::exists(modelsPath) || !filesystem::is_directory(modelsPath))
    {
        printf("경로를 찾을 수 없습니다: %s\n", modelsPath.c_str());
        return;
    }

    for (const auto& entry : filesystem::directory_iterator(modelsPath))
    {
        if (entry.is_regular_file())
        {
            string filePath = entry.path().string();

            if (entry.path().extension() != ".txt")
            {
                continue;
            }

            string id = entry.path().stem().string();

            if (LoadModelData(id, filePath))
            {
                printf("로드 성공: [%s] -> %s\n", id.c_str(), filePath.c_str());
            }
        }
    }
}

bool ResourceManager::LoadModelData(string id, string filename)
{
    if (m_models.find(id) != m_models.end())
    {
        return true;
    }

    // 오픈
    ifstream fin;

    fin.open(filename);
    if (fin.fail())
    {
        return false;
    }

    ModelData* newModel = new ModelData();

    // 파일 읽기
    while (!fin.eof())
    {
        ModelType tempVertex;

        if (!(fin >> tempVertex.x >> tempVertex.y >> tempVertex.z >>
            tempVertex.tu >> tempVertex.tv >>
            tempVertex.nx >> tempVertex.ny >> tempVertex.nz))
        {
            break;
        }

        newModel->vertices.push_back(tempVertex);
    }

    fin.close();

    if (newModel->vertices.empty())
    {
        delete newModel;
        return false;
    }

    m_models.insert(make_pair(id, newModel));

    return true;
}

ModelData* ResourceManager::GetModel(string id)
{
    auto it = m_models.find(id);
    if (it != m_models.end())
    {
        return it->second;
    }
    return nullptr;
}

void ResourceManager::Shutdown()
{
    for (auto& pair : m_models)
    {
        if (pair.second)
        {
            pair.second->vertices.clear();
            delete pair.second;
            pair.second = nullptr;
        }
    }
    m_models.clear();
}