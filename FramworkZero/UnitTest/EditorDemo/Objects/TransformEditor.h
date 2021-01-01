#pragma once

class TransformEditor
{
public:
	TransformEditor();
	~TransformEditor();

	void RenderImGui(Transform* t);
	void Save(Transform* t, class BinaryWriter * w);
	void Load(Transform* t, class BinaryReader * r);

private:

};