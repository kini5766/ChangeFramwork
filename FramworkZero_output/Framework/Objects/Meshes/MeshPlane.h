#pragma once

class MeshPlane : public MeshData
{
public:
	MeshPlane(float offsetU = 1.0f, float offsetV = 1.0f);
	~MeshPlane();

private:
	// 스케일 늘려도 깨지지 않게(타일형식으로)
	float offsetU, offsetV;
};