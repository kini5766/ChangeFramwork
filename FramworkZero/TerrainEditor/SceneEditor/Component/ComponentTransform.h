#pragma once

class ComponentTransform
{
public:
	ComponentTransform();
	~ComponentTransform();

	void RenderImGui(Transform* t);
	void Save(Transform* t, class BinaryWriter * w);
	void Load(Transform* t, class BinaryReader * r);

private:

};