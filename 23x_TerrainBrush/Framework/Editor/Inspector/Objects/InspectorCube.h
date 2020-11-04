#pragma once

#include "../IInspectorEditor.h"

class InspectorCube : public IInspectorEditor
{
public:
	InspectorCube(Shader* shader);
	~InspectorCube();

public:
	// IInspectorEditor��(��) ���� ��ӵ�
	void On() override;
	void Off() override;
	void AddSceneValue(SceneValue* value) override;

	void Render() override;
	void Save(Xml::XMLDocument * document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;

private:
	Shader* shader;
	MeshCube* cube;

	UINT pass = 0;
};