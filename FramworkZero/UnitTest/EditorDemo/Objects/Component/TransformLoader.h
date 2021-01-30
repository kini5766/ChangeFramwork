#pragma once

class TransformLoader
{
public:
	TransformLoader();
	~TransformLoader();

	void Load(Transform* t, class BinaryReader * r);
};