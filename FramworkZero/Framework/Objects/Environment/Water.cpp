#include "Framework.h"
#include "Water.h"

#include "Rendering/Projection/Reflection.h"
#include "Rendering/Projection/Refraction.h"

using namespace ShaderEffectName;

Water::Water(const InitializeDesc & initDesc)
	: radius(initDesc.Radius)
{
	shader = Shader::Load(L"01_Water.fxo");

	mesh = new Mesh(shader, MakeMeshData(initDesc.Radius));
	buffer = new ConstantBuffer(&desc, sizeof(Desc));

	reflection = new Reflection(shader, mesh->GetTransform(), initDesc.Width, initDesc.Height);
	refraction = new Refraction(shader, mesh->GetTransform(), initDesc.Width, initDesc.Height);
	
	mesh->GetMaterial()->NormalMap(L"Environment/Wave1.dds");
	mesh->GetMaterial()->SetConstantBuffer(CB_WATER, buffer->Buffer());
}

Water::~Water()
{
	SafeDelete(buffer);
	SafeDelete(refraction);
	SafeDelete(reflection);

	SafeDelete(mesh);
	SafeRelease(shader);
}

void Water::Update()
{
	mesh->Update();

	desc.WaveTransform += waveSpeed * Time::Delta();
	if (desc.WaveTransform > 1.0f)
		desc.WaveTransform -= 2.0f;
	else if (desc.WaveTransform < -1.0f)
		desc.WaveTransform += 2.0f;

	reflection->Update();
}

void Water::Render()
{
	reflection->Render();
	refraction->Render();

	buffer->Render();

	mesh->Render();
}

void Water::PreRender_Reflection()
{
	reflection->PreRender();
}

void Water::PreRender_Refraction()
{
	refraction->PreRender();
}

MeshData Water::MakeMeshData(float radius)
{
	MeshData data;
	{
		VertexTexture* vertices = data.NewVertices<VertexTexture>(4);
		vertices[0].Position = Vector3(-radius, 0.0f, -radius);
		vertices[1].Position = Vector3(-radius, 0.0f, +radius);
		vertices[2].Position = Vector3(+radius, 0.0f, -radius);
		vertices[3].Position = Vector3(+radius, 0.0f, +radius);

		vertices[0].Uv = Vector2(0, 1);
		vertices[1].Uv = Vector2(0, 0);
		vertices[2].Uv = Vector2(1, 1);
		vertices[3].Uv = Vector2(1, 0);

		UINT indices[6] = { 0,1,2,2,1,3 };
		data.SetIndices(indices, 6);
	}
	return data;
}

