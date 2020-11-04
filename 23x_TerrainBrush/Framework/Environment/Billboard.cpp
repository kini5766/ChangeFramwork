#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(Shader * shader, wstring file)
	: shader(shader)
{
	texture = new Texture(file);
	quad = new MeshQuad(shader);
	sMap = shader->AsSRV("DiffuseMap");
}

Billboard::~Billboard()
{
	SafeDelete(texture);
	SafeDelete(quad);
}

void Billboard::Update()
{
	Matrix V = Context::Get()->View();
	D3DXMatrixInverse(&V, nullptr, &V);

	// LU Decompose
	float x = 0;
	x = asin(-V._32);
	float z = atan2f(V._12, V._22);
	float y = atan2f(V._31, V._33);
	//quad->GetTransform()->Rotation(x, y, z);
	quad->GetTransform()->Rotation(0.0f, y, 0.0f);

	quad->Update();
}

void Billboard::Render()
{
	sMap->SetResource(texture->SRV());
	quad->Render();
}
