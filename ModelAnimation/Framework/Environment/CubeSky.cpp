#include "Framework.h"
#include "CubeSky.h"

CubeSky::CubeSky(wstring file)
{
	shader = new Shader(L"11_CubeSky.fx");

	sphere = new MeshSphere(shader, 0.5f);
	//sphere->GetTransform()->Scale(200, 1200, 3000);

	wstring temp = URI::Textures + file;
	Check(D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), temp.c_str(), nullptr, nullptr, &srv, nullptr));

	sSrv = shader->AsSRV("SkyCubeMap");
}

CubeSky::~CubeSky()
{
	SafeDelete(shader);
	SafeDelete(sphere);
	SafeRelease(srv);
}

void CubeSky::Update()
{
	Vector3 position;
	Context::Get()->GetCamera()->Position(&position);

	sphere->GetTransform()->Position(position);
}

void CubeSky::Render()
{
	sSrv->SetResource(srv);
	sphere->Render();
}
