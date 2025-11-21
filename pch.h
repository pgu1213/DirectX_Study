
#ifndef PCH_H
#define PCH_H

// STL
#include <iostream>
#include <filesystem>
#include <fstream>
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

#include "framework.h"
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <typeindex>

using namespace std;

// DirectX
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

using namespace DirectX;

// Header Files
#include "Header/Define.h"
#include "Header/Extern.h"
#include "Header/Struct.h"

#endif