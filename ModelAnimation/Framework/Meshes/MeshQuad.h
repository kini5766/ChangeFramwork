#pragma once

#include "MeshData.h"

class MeshQuad : public Mesh
{
public:
	MeshQuad(Shader* shader);
	~MeshQuad();

	void Create() override;
};