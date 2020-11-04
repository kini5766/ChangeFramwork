#pragma once

#include "Framework.h"
#pragma comment(lib, "Framework.lib")

// -> 미리 컴파일러를 보라고하면 Framework.h로 이동

#include "Assimp/Importer.hpp"
#include "Assimp/scene.h"  // 씬
#include "Assimp/postprocess.h"  // 어떻게 읽을지 옵션
#pragma comment(lib, "Assimp/assimp-vc140-mt.lib")
