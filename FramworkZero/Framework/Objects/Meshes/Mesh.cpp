#include "Framework.h"
#include "Mesh.h"

Mesh::Mesh(Shader * shader, MeshData && meshMove)
{
	MeshData::Move(&data, &meshMove);
	renderer = new MeshRenderer(shader, &data);
	perTransform = new PerTransform(shader);

	material = renderer->GetDefaultMaterial();
	transform = perTransform->GetTransform();
}

Mesh::~Mesh()
{
	SafeDelete(perTransform);
	SafeDelete(renderer);
}

void Mesh::Update()
{
	perTransform->Update();
}

void Mesh::Render()
{
	perTransform->Render();
	renderer->Render();
}
