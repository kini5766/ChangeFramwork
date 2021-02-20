#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard()
{
	shader = new ShaderSetter(Shader::Load(URI::Shaders + L"01_Billboard.fxo"));
	perFrame = new PerFrame();
	perFrame->SetAtMaterial(shader);

	preEnvCube = Context::Get()->AddEnvCubeCaster({ shader->GetShader(),
		bind(&Billboard::PreRender_EnvCube, this) }
	);
}

Billboard::~Billboard()
{
	SafeDelete(shader);
	SafeDelete(perFrame);
	SafeDelete(textureArray);

	SafeRelease(preEnvCube);
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
	shader->GetShader()->Draw(0, 0, vertexCount);
}

void Billboard::PreRender_EnvCube()
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
	shader->GetShader()->Draw(0, 1, vertexCount);
}
