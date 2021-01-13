#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(Shader* _shader)
{
	shader = new ShaderSetter(_shader);
	perFrame = new PerFrame(_shader);
}

Billboard::~Billboard()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(textureArray);
}

void Billboard::SetTextures(vector<wstring> value)
{
	swap(textureFiles, value);
	SafeDelete(textureArray);

	textureArray = new TextureArray(textureFiles);
	shader->SetSRV("BillboardMaps", textureArray->SRV());
}

void Billboard::Add(const Vector3 & position, const Vector2 & scale, UINT mapIndex)
{
	vertices.push_back({ position, scale, mapIndex });
}


void Billboard::Update()
{
	perFrame->Update();
}

void Billboard::Render()
{
	if (vertices.size() != vertexCount)
	{
		vertexCount = vertices.size();
		SafeDelete(vertexBuffer);
		vertexBuffer = new VertexBuffer(vertices.data(), vertices.size(), sizeof(VertexBillboard));
	}

	perFrame->Render();
	vertexBuffer->Render();
	shader->Render();
	shader->GetShader()->Draw(0, 2, vertexCount);
}
