#pragma once

#include "Utilities/BinaryFile.h"
#include "../Main/SceneValue.h"

class IObjectSpawner
{
public:
	virtual ~IObjectSpawner() {}

	// return : 로드 성공 여부
	virtual bool Load(BinaryReader* r) = 0;

	// 모든 로드 뒤에 호출
	virtual void Initialize(SceneValueUnit* scene) = 0;
};
