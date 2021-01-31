#pragma once

class MeshInstancingImGui
{
public:
	MeshInstancingImGui(class MeshInstancingDesc* desc);
	~MeshInstancingImGui();

public:
	void ImGuiRender();
	void Select(int i);
	void SetImguiItem(int value) { imguiItem = value; }

private:
	void AddInstance();
	void MakingInstancing();


private:
	class MeshInstancingDesc* desc;

	// imgui
private:
	int selected = -1;
	int imguiItem = 0;

private:
	const char* meshItems = {
		"Cube\0"
		"Cylinder\0"
		"Plane\0"
		"Quad\0"
		"Sphere\0"
	};
};