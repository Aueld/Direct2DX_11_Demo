#pragma once

#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

//미리 컴파일된 헤더.
//pch pre compiled header

#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <fstream>

using namespace std;

//////////////////////////////////////////////
//DirectX
#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#include <d3dcompiler.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dcompiler.lib")
//////////////////////////////////////////
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
//////////////////////////////////////////
//ImGui
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#pragma comment(lib, "ImGui/ImGui.lib")

//////////////////////////////////////////
// XAudio2
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
//////////////////////////////////////////
// FMOD
#include "../_Libraries/FMOD/inc/fmod.hpp"
#ifdef _WIN64
#pragma comment(lib, "../_Libraries/FMOD/lib/x64/fmod_vc.lib")
#else
#pragma comment(lib, "../_Libraries/FMOD/lib/x86/fmod_vc.lib")
#endif

typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR2 Vector2;
typedef D3DXMATRIX  Matrix;
typedef D3DXCOLOR   Color;
typedef UINT		uint;

#include "Interfaces/IObject.h"

#include "Utilities/SingletonBase.h"

#include "Systems/Graphics.h"
#include "Systems/Keyboard.h"
#include "Systems/Mouse.h"
#include "Systems/Time.h"
#include "Systems/Sound.h"
#include "Systems/Text.h"
#include "Systems/SRV.h"

#include "Utilities/Gui.h"

#define DEVICE      Graphics::Get()->GetDevice()
#define DC          Graphics::Get()->GetDC()

#define GRAPHICS    Graphics::Get()
#define KEYBOARD    Keyboard::Get()
#define MOUSE       Mouse   ::Get()
#define TIME        Time    ::Get()
#define CAMERA      Camera  ::Get()
#define SOUND       Sounds  ::Get()
#define TXT         Text    ::Get()

#define CHECK(p) assert(SUCCEEDED(p))
#define ZERO(p) ZeroMemory(&p, sizeof(p))

#define SAFE_DELETE(p)		 { if(p) { delete(p);	   (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[](p);	   (p) = nullptr; } }
#define SAFE_RELEASE(p)		 { if(p) { (p)->Release(); (p) = nullptr; } }

#include "Utilities/DirectHelper.h"

#include "Renders/Resources/VertexType.h"
#include "Renders/Resources/ShaderBuffer.h"
#include "Renders/Resources/GlobalBuffer.h"
#include "Renders/Resources/Texture2D.h"

#include "Renders/IA/VertexBuffer.h"
#include "Renders/IA/IndexBuffer.h"
#include "Renders/IA/InputLayout.h"
#include "Renders/States.h"

#include "Renders/Shaders/VertexShader.h"
#include "Renders/Shaders/PixelShader.h"

#include "Utilities/BoundingBox.h"
#include "Utilities/String.h"
#include "Utilities/Path.h"

#include "Systems/Camera.h"

#define WinMaxWidth 1280
#define WinMaxHeight 720

const wstring ShaderPath = L"../_Shaders/";
const wstring TexturePath = L"../_Resources/_Textures/";
const wstring CharacterPath = L"../_Resources/_Textures/_Character/";
const wstring SoundPath = L"../_Resources/_Sounds/";
const string  TilePath = "../_Resources/_TileDatas/";

extern HWND handle;