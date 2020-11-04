#pragma once

#include "../IInspectorEditor.h"

class InspectorModel : public IInspectorEditor
{
	const char* modelItem = {
		"Input\0"
		"Aiplane\0"
		"Tower\0"
		"Tank\0"
		"Kachujin\0"
	};

	wstring materialItems[5] = {
		L"",
		L"B787/Airplane",
		L"Tower/Tower",
		L"Tank/Tank",
		L"Kachujin/Mesh"
	};

	wstring meshItems[5] = {
		L"",
		L"B787/Airplane",
		L"Tower/Tower",
		L"Tank/Tank",
		L"Kachujin/Mesh"
	};


public:
	InspectorModel(Shader* shader);
	~InspectorModel();

public:
	// IInspectorEditor을(를) 통해 상속됨
	void On() override;
	void Off() override;
	void AddSceneValue(SceneValue* value) override;

	void Render() override;
	void Save(Xml::XMLDocument * document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;

public:
	void Model(ModelRender * value) { model = value; }

private:
	void CreateModel();

private:
	Shader* shader;

	ModelRender* model = nullptr;

	class InputTextImGui* inputMaterial;
	class InputTextImGui* inputMesh;
	int item = 0;
	UINT pass = 1;
};