#include "Framework.h"
#include "PerFrame.h"

using namespace ShaderEffctConstantName;

PerFrame::PerFrame(Shader * shader)
{
	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	sBuffer = shader->AsConstantBuffer(CB_PerFrame);

	lightBuffer = new ConstantBuffer(&lightDesc, sizeof(LightDesc));
	sLightBuffer = shader->AsConstantBuffer(CB_Light);

	// bufferDesc
	// lightDesc
}

PerFrame::~PerFrame()
{
	SafeDelete(buffer);
	SafeDelete(lightBuffer);
}

void PerFrame::Update()
{
	bufferDesc.Time = Time::Get()->Running();

	lightDesc.Ambient = Context::Get()->Ambient();
	lightDesc.Specular = Context::Get()->Specular();
	lightDesc.Direction = Context::Get()->Direction();
	lightDesc.Position = Context::Get()->Position();
}

void PerFrame::Render()
{
	bufferDesc.View = Context::Get()->View();
	bufferDesc.Projection = Context::Get()->Projection();
	bufferDesc.VP = bufferDesc.View * bufferDesc.Projection;
	D3DXMatrixInverse(&bufferDesc.ViewInverse, nullptr, &bufferDesc.View);

	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	lightBuffer->Render();
	sLightBuffer->SetConstantBuffer(lightBuffer->Buffer());
}
