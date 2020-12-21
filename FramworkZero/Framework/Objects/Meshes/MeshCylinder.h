#pragma once

//Todo 14. Cylinder 작성
class MeshCylinder : public MeshData
{
public:
	// 반경(원크기)radius, 높이 height, 층 정점 stackCount, 세로 정점(원 정점 개수) sliceCount
	MeshCylinder(float radius, float height, UINT stackCount, UINT sliceCount);
	// 위아래 원크기 다르게
	MeshCylinder(float topRadius, float bottomRadius, float height, UINT stackCount, UINT sliceCount);
	~MeshCylinder();

private:
	void Create();
	void BuildTopCap(vector<VertexMesh>& vertices, vector<UINT>& indices);
	void BuildBottomCap(vector<VertexMesh>& vertices, vector<UINT>& indices);

private:
	float topRadius;
	float bottomRadius;
	float height;


	UINT stackCount;
	UINT sliceCount;
};