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

	preEnvCube = Context::Get()->AddEnvCubeCaster({ shader,
		bind(&CubeSky::PreRender_EnvCube, this) }
	);
}

CubeSky::~CubeSky()
{
	material->GetShader()->Release();
	SafeDelete(sphere);

	SafeRelease(srv);
	SafeRelease(preEnvCube);
}

void CubeSky::Update()
{
	sphere->Update();
}

void CubeSky::Render()
{
	Matrix V = Context::Get()->View();
	D3DXMatrixInverse(&V, nullptr, &V);
	transform->Position(V._41, V._42, V._43);

	sphere->Pass(0);
	sphere->Render();
}

void CubeSky::PreRender_EnvCube()
{
	Matrix V = Context::Get()->View();
	D3DXMatrixInverse(&V, nullptr, &V);
	transform->Position(V._41, V._42, V._43);

	sphere->Pass(1);
	sphere->Render();
}
