#pragma once

class MeshPlane : public MeshData
{
public:
	MeshPlane(float offsetU = 1.0f, float offsetV = 1.0f);
	~MeshPlane();

private:
	// ������ �÷��� ������ �ʰ�(Ÿ����������)
	float offsetU, offsetV;
};