#pragma once

#include "Mesh.h"

class MeshQuad : public Mesh
{
public:
	MeshQuad(Shader* shader);
	~MeshQuad();

	// Mesh��(��) ���� ��ӵ�
	void Create() override;
};