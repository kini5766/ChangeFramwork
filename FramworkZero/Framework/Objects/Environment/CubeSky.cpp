#include "Framework.h"
#include "CubeSky.h"

CubeSky::CubeSky(wstring file_cubeMap)
{
	Shader* shader = Shader::Load(URI::Shaders + L"01_SkyCube.fxo");

	sphere = new Mesh(shader, MeshSphere(0.5f));
	sphere->Pass(0);
	transform = sphere->GetTransform();
	//transform->Scale(500, 500, 500);

	wstring file = URI::Textures + file_cubeMap;
	Check(D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), file.c_str(), NULL, NULL, &srv, NULL
	));

	material = sphere->GetMaterial();
	material->SetSRV("SkyCubeMap", srv);
}

CubeSky::~CubeSky()
{
	SafeRelease(srv);

	material->GetShader()->Release();
	SafeDelete(sphere);
}

void CubeSky::Update()
{
	Vector3 position;
	Context::Get()->MainCamera()->GetTransform()->Position(&position);

	transform->Position(position);
	sphere->Update();
}

void CubeSky::Render()
{
	sphere->Render();
}
