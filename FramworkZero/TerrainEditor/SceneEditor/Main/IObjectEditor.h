#pragma once

#include "Utilities/BinaryFile.h"
#include "E_SceneValue.h"

class IObjectEditor
{
public:
	virtual ~IObjectEditor() {}
	virtual void ImGuiRender() = 0;
	virtual void Initialize(E_SceneValue* value) = 0;
	virtual void On() {}
	virtual void Off() {}

	virtual void Save(BinaryWriter* w) = 0;
	virtual void Load(BinaryReader* r) = 0;
	// return : 로드 성공 여부
};
