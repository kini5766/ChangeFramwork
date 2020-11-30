#pragma once

#include "../IInspectorEditor.h"

class InspectorAnimator : public IInspectorEditor
{
	const char* modelItem = {
		"Input\0"
		"Kachujin\0"
		"Paladin\0"
	};

	wstring materialItems[3] = {
		L"",
		L"Kachujin/Mesh",
		L"Paladin/Mesh",
	};

	wstring meshItems[3] = {
		L"",
		L"Kachujin/Mesh",
		L"Paladin/Mesh",
	};


public:
	InspectorAnimator(Shader* shader);
	~InspectorAnimator();

public:
	// IInspectorEditor을(를) 통해 상속됨
	void On() override;
	void Off() override;
	void AddSceneValue(SceneValue* value) override;

	void Render() override;
	void Save(Xml::XMLDocument * document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;

public:
	void Model(ModelAnimator * value) { model = value; }

private:
	void CreateModel();
	void RenderClipInputs(string label);

private:
	Shader* shader;

	ModelAnimator* model = nullptr;

	class InputTextImGui* inputMaterial;
	class InputTextImGui* inputMesh;
	vector<InputTextImGui*> inputClips;
	int item = 0;
	UINT pass = 2;

	string clipItem = "";
	int clip = 0;
};