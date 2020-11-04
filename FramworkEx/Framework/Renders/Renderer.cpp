#include "Framework.h"
#include "Renderer.h"

Renderer::Renderer(Shader * shader)
	: shader(shader), bCreateShader(false)
{
	Initialize();
	transform->SetShader(shader);
}

Renderer::Renderer(wstring shaderFile)
	: bCreateShader(true)
{
	shader = new Shader(shaderFile);
	Initialize();
}

Renderer::~Renderer()
{
	SafeDelete(perFrame);
	SafeDelete(transform);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	if (bCreateShader)
		SafeDelete(shader);
}

void Renderer::Update()
{
	perFrame->Update();
	transform->Update();
}

void Renderer::Render()
{
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Render();

		if (indexBuffer != nullptr)
			indexBuffer->Render();
	}

	D3D::GetDC()->IASetPrimitiveTopology(topology);

	perFrame->Render();
	transform->Render();
}

void Renderer::Initialize()
{
	perFrame = new PerFrame(shader);
	transform = new Transform(shader);
}
