#pragma once

#include "Framework.h"
#pragma comment(lib, "../Debug/Framework.lib")

// �����̳ʰ� ���� �� ���� �ҷ���
#include "Assimp/Importer.hpp"  

// �ҽ��� �ҷ��� �� �پ��� �ɼ��� �� �� ����
#include "Assimp/postprocess.h"  

// postprocess�� ���� �ҷ��� ���� (RootNode-(bone,tree), Meshes, Material, Animation �� -> �迭 ���·�)
#include "Assimp/scene.h"  

#pragma comment(lib, "Assimp/assimp-vc140-mt.lib")

// assimp-vc140-mt.dll
// ���� ./�� ����