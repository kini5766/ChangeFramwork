#pragma once

class MeshSphere : public MeshData
{
public:
	MeshSphere(float radius, UINT stackCount = 20, UINT sliceCount = 20);
	~MeshSphere();

private:
	float radius;

	UINT stackCount;
	UINT sliceCount;
};