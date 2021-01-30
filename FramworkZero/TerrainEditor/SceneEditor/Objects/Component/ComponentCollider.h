#pragma once

class ComponentCollider
{
public:
	ComponentCollider();
	~ComponentCollider();

	void RenderImGui(ColliderBox* c);
	void Save(ColliderBox* c, class BinaryWriter * w);
	void Load(ColliderBox* c, class BinaryReader * r);

private:
	int inputMask = 0;
};