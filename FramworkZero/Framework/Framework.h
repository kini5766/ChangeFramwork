#pragma once

#include <Windows.h>
#include <assert.h>

//STL
#include <string>
#include <vector>
#include <list>
#include <bitset>
#include <map>
#include <unordered_map>
#include <functional>
#include <iterator>
#include <thread>
#include <mutex>
#include <queue>
using namespace std;

//Direct3D
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <d3dx11effect.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Effects11d.lib")

//ImGui
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/ImGuizmo.h>
#pragma comment(lib, "ImGui/imgui.lib")

//DirectXTex
#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

#define Check(hr) { assert(SUCCEEDED(hr)); }
#define Super __super

#define SafeRelease(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SafeDelete(p){ if(p){ delete (p); (p) = NULL; } }
#define SafeDeleteArray(p){ if(p){ delete [] (p); (p) = NULL; } }

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXCOLOR Color;
typedef D3DXMATRIX Matrix;
typedef D3DXQUATERNION Quaternion;
typedef D3DXPLANE Plane;


// Settings
// 실행 전 셋팅할 값들

#include "Settings/URI.h"
#include "Settings/ShaderEffectName.h"


// Systems
// 윈도우, d3d, imgui 생성

#include "Systems/D3D.h"
#include "Systems/Gui.h"


// Tools
// 필요할 때 꺼내 쓰는 클래스들 (특징 : Rendering폴더 클래스를 사용하지 않음)

#include "Tools/MainLogic/Keyboard.h"
#include "Tools/MainLogic/Mouse.h"
#include "Tools/MainLogic/Time.h"
//#include "Tools/MainLogic/GameLogic.h"

#include "Tools/Math.h"
#include "Tools/WinDesc.h"
#include "Tools/Transform.h"

#include "Tools/Texture.h"
#include "Tools/Texture2D.h"

#include "Tools/Viewer/Viewport.h"
#include "Tools/Viewer/Projection.h"
#include "Tools/Viewer/Perspective.h"
#include "Tools/Viewer/Orthographic.h"
#include "Tools/Viewer/Camera.h"
#include "Tools/Viewer/Freedom.h"

#include "Tools/Lighting/LightingDesc.h"
#include "Tools/Lighting/DirectionalLight.h"
#include "Tools/Lighting/PointLight.h"
#include "Tools/Lighting/SpotLight.h"
#include "Tools/Lighting/BurntLight.h"
#include "Tools/Lighting/Lighting.h"

#include "Tools/Animation/ClipTimer.h"
#include "Tools/Animation/Animator.h"

#include "Tools/Collider/Ray.h"
#include "Tools/Collider/Collider.h"
#include "Tools/Collider/CollisionManager.h"

//Rendering
// IASet하거나, Shader에 값 넘겨주는 클래스들

#include "Rendering/Shader.h"
#include "Rendering/Buffers/Buffers.h"

#include "Rendering/CBufferVariables/Context.h"
#include "Rendering/CBufferVariables/ShaderSetter.h"
#include "Rendering/CBufferVariables/Material.h"
#include "Rendering/CBufferVariables/PerFrame.h"


// Utilities
// 유용한

#include "Utilities/String.h"
#include "Utilities/Path.h"


// Objects
// Rendering클래스를 이용해서 물체를 그리는 클래스

#include "Objects/Meshes/MeshData.h"
#include "Objects/Meshes/MeshQuad.h"
#include "Objects/Meshes/MeshPlane.h"
#include "Objects/Meshes/MeshCube.h"
#include "Objects/Meshes/MeshCylinder.h"
#include "Objects/Meshes/MeshSphere.h"
#include "Objects/Meshes/MeshRenderer.h"
#include "Objects/Meshes/MeshInstancing.h"
#include "Objects/Meshes/SkinnedMeshRenderer.h"

#include "Objects/Model/ModelData.h"
#include "Objects/Model/ModelAnimationDesc.h"
#include "Objects/Model/ModelSkinnedInstancing.h"


// Debugger
// 디버그용 클래스들

#include "Debugger/DebugLog.h"
#include "Debugger/DebugLine.h"
#include "Debugger/DebugBox.h"
#include "Debugger/Gizmo.h"

