#pragma once

class ColliderEditor
{
public:
	ColliderEditor();
	~ColliderEditor();

	void RenderImGui(ReceiveBox* c);
	void Save(ReceiveBox* c, class BinaryWriter * w);
	void Load(ReceiveBox* c, class BinaryReader * r);

private:
	int inputMask = 0;
};