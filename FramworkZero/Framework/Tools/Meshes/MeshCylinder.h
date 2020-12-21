#pragma once

//Todo 14. Cylinder �ۼ�
class MeshCylinder : public MeshData
{
public:
	// �ݰ�(��ũ��)radius, ���� height, �� ���� stackCount, ���� ����(�� ���� ����) sliceCount
	MeshCylinder(float radius, float height, UINT stackCount, UINT sliceCount);
	// ���Ʒ� ��ũ�� �ٸ���
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