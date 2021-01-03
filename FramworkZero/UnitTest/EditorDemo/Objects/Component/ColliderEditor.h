#pragma once

class ColliderEditor
{
public:
	ColliderEditor();
	~ColliderEditor();

	void RenderImGui(ColliderBox* c);
	void Save(ColliderBox* c, class BinaryWriter * w);
	void Load(ColliderBox* c, class BinaryReader * r);

private:
	int inputMask = 0;
};