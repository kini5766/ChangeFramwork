#include "Framework.h"
#include "CubeSky.h"

CubeSky::CubeSky(wstring file_cubeMap, Shader* _shader)
{
	shader = new ShaderSetter(_shader);

	sphere = new MeshInstancing(this->shader->GetShader(), new MeshSphere(0.5f));
	transform = sphere->AddInstance()->GetTransform();
	transform->Scale(500, 500, 500);

	wstring file = URI::Textures + file_cubeMap;
	Check(D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), file.c_str(), NULL, NULL, &srv, NULL
	));

	shader->SetSRV("SkyCubeMap", srv);

	sphere->Pass(3);
}

CubeSky::~CubeSky()
{
	SafeRelease(srv);

	SafeDelete(sphere);
	SafeDelete(shader);
}

void CubeSky::Update()
{
	Vector3 position;
	Context::Get()->MainCamera()->Position(&position);

	transform->Position(position);
	sphere->UpdateTransforms();
	sphere->Update();
}

void CubeSky::Render()
{
	shader->Render();
	sphere->Render();
}
