#pragma once

#include "Utilities/BinaryFile.h"
#include "../Main/SceneValue.h"

class IObjectSpawner
{
public:
	virtual ~IObjectSpawner() {}

	// return : �ε� ���� ����
	virtual bool Load(BinaryReader* r) = 0;

	// ��� �ε� �ڿ� ȣ��
	virtual void Initialize(SceneValueUnit* scene) = 0;
};
