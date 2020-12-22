#pragma once

#include "Framework.h"
#pragma comment(lib, "../Debug/Framework.lib")

// 디자이너가 만든 모델 파일 불러옴
#include "Assimp/Importer.hpp"  

// 소스로 불러올 때 다양한 옵션을 줄 수 있음
#include "Assimp/postprocess.h"  

// postprocess를 통해 불러온 값들 (RootNode-(bone,tree), Meshes, Material, Animation 등 -> 배열 형태로)
#include "Assimp/scene.h"  

#pragma comment(lib, "Assimp/assimp-vc140-mt.lib")

// assimp-vc140-mt.dll
// 파일 ./로 복사