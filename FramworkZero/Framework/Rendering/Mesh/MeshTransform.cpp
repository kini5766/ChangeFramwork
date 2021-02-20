#include "Framework.h"
#include "MeshTransform.h"

Mesh::Mesh(Shader* shader, MeshData && meshMove)
{
	MeshData::Move(&data, &meshMove);

	material = new Material(shader);
	renderer = new MeshRenderer(&data);
	perTransform = new PerTransform();

	perTransform->SetAtMaterial(material);
	renderer->SetMaterial(material);
	transform = perTransform->GetTransform();
}

Mesh::~Mesh()
{
	SafeDelete(perTransform);
	SafeDelete(renderer);
	SafeDelete(material);
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
