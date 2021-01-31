#pragma once

class ColliderLoader
{
public:
	ColliderLoader();
	~ColliderLoader();

	void Load(ColliderBox* c, class BinaryReader * r);
};