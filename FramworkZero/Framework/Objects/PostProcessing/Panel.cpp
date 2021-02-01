#include "Framework.h"
#include "Panel.h"

Panel::Panel(Shader* shader)
{
	material = new ShaderSetter(shader);
	perFrame = new PerFrame(shader);

	Vertex verteces[6];
	verteces[0].Position = Vector3(-1.0f, -1.0f, 0.0f);
	verteces[1].Position = Vector3(-1.0f, +1.0f, 0.0f);
	verteces[2].Position = Vector3(+1.0f, -1.0f, 0.0f);
	verteces[3].Position = Vector3(+1.0f, -1.0f, 0.0f);
	verteces[4].Position = Vector3(-1.0f, +1.0f, 0.0f);
	verteces[5].Position = Vector3(+1.0f, +1.0f, 0.0f);

	vertexBuffer = new VertexBuffer(verteces, 6, sizeof(Vertex));
}

Panel::~Panel()
{
	SafeDelete(perFrame);
	SafeDelete(material);
	SafeDelete(vertexBuffer);
}

void Panel::Update()
{
	perFrame->Update();
}

void Panel::Render()
{
	perFrame->Render();

	vertexBuffer->Render();

	material->Render();
	material->GetShader()->Draw(0, pass, 6);
}

void Panel::SRV(ID3D11ShaderResourceView * srv)
{
	material->SetSRV("DiffuseMap", srv);
}
