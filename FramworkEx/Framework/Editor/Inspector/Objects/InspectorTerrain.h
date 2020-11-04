#pragma once

#include "../IInspectorEditor.h"

class InspectorTerrain : public IInspectorEditor
{
public:
	InspectorTerrain(Shader* shader);
	~InspectorTerrain();

public:
	// IInspectorEditor을(를) 통해 상속됨
	void On() override;
	void Off() override;
	void AddSceneValue(SceneValue* value) override;

	void Render() override;
	void Save(Xml::XMLDocument * document, Xml::XMLElement * node) override;
	void Load(Xml::XMLElement * node) override;

	void CreateTerrain(const wstring& heightMap, const wstring& baseMap);
	void HeightMap(const char* value);
	void BaseMap(const char* value);

private:
	Shader* shader;

	class Terrain* terrain = nullptr;
	char heightMap[28] = { "Terrain/Gray256.png" };
	char baseMap[28] = { "Terrain/Dirt3.png" };

	UINT pass = 0;
	int item = 0;
};

// Terrain/Dirt3.png