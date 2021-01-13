#include "Framework.h"
#include "PostEffect.h"

PostEffect::PostEffect(wstring shaderFile)
{
	shader = new ShaderSetter(shaderFile);

	Vertex verteces[6];
	verteces[0].Position = Vector3(-1.0f, -1.0f, 0.0f);
	verteces[1].Position = Vector3(-1.0f, +1.0f, 0.0f);
	verteces[2].Position = Vector3(+1.0f, -1.0f, 0.0f);
	verteces[3].Position = Vector3(+1.0f, -1.0f, 0.0f);
	verteces[4].Position = Vector3(-1.0f, +1.0f, 0.0f);
	verteces[5].Position = Vector3(+1.0f, +1.0f, 0.0f);

	vertexBuffer = new  VertexBuffer(verteces, 6, sizeof(Vertex));
	shader->SetSRV("DiffuseMap", nullptr);
}

PostEffect::~PostEffect()
{
	SafeDelete(shader);
	SafeDelete(vertexBuffer);
}

void PostEffect::Update()
{
}

void PostEffect::Render()
{
	vertexBuffer->Render();
	shader->Render();

	shader->GetShader()->Draw(0, pass, 6);
}

void PostEffect::SRV(ID3D11ShaderResourceView * srv)
{
	shader->SetSRV("DiffuseMap", srv);
}
