#pragma once

class ColliderEditor
{
public:
	ColliderEditor();
	~ColliderEditor();

	void RenderImGui(Collider* c);
	void Save(Collider* c, class BinaryWriter * w);
	void Load(Collider* c, class BinaryReader * r);

private:
	int inputMask = 0;
};