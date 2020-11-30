#pragma once

#include "Framework.h"
#include "Utilities/Xml.h"
#include "../SceneValue.h"

class IInspectorEditor
{
public:
	virtual void Render() = 0;
	virtual void Save(Xml::XMLDocument* document, Xml::XMLElement* node) = 0;
	virtual void Load(Xml::XMLElement* node) = 0;

	virtual void On() {}
	virtual void Off() {}
	virtual void AddSceneValue(SceneValue* value) {}
};

// char name[255];
//wchar_t current;
//int S(ImGuiInputTextCallbackData* d)
//{
//	switch (d->EventFlag)
//	{
//	case ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackCharFilter:
//		current = d->EventChar;
//		break;
//	default:
//		break;
//	}
//
//	return 0;
//}
//ImGuiInputTextCallback cb = S;