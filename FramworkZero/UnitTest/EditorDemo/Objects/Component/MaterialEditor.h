#pragma once

class MaterialEditor
{
public:
	MaterialEditor();
	~MaterialEditor();

	void RenderImGui();
	void Save(class BinaryWriter * w);
	void Load(class BinaryReader * r);

	void Apply(Material* mat);

private:
	class ImGuiInputText* diffuseImGui;
	class ImGuiInputText* specularImGui;
	class ImGuiInputText* normalMapImGui;

private:
	Color ambient = Color(1, 0, 1, 1);
	Color emissive = Color(0, 0, 0, 1);
	Color diffuse = Color(1, 1, 1, 1);
	wstring diffuseMap = L"";
	Color specular = Color(0, 0, 0, 1);
	wstring specularMap = L"";
	wstring normalMap = L"";
};