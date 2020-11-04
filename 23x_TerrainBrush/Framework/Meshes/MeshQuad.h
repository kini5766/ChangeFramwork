#pragma once

#include "Mesh.h"

class MeshQuad : public Mesh
{
public:
	MeshQuad(Shader* shader);
	~MeshQuad();

	// Mesh을(를) 통해 상속됨
	void Create() override;
};