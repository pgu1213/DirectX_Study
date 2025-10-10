#include <pch.h>
#include "PrototypeFactory.h"
#include <Engine/Manager/PrototypeManager/PrototypeManager.h>

unique_ptr<Object> PrototypeFactory::CreateFromPrototype(const string& prototypeName)
{
    return PrototypeManager::GetInstance()->CreateFromPrototype(prototypeName);
}

void PrototypeFactory::CreateDefaultPrototypes()
{

}
