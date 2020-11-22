#include "Framework.h"
#include "PerFrame.h"

using namespace ShaderEffctConstantName;

// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

PerFrameBuffer::PerFrameBuffer()
{
	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	lightBuffer = new ConstantBuffer(&lightDesc, sizeof(LightDesc));
}

PerFrameBuffer::~PerFrameBuffer()
{
	SafeDelete(buffer);
	SafeDelete(lightBuffer);
}

void PerFrameBuffer::Update()
{
	bufferDesc.Time = Time::Get()->Running();

	lightDesc.Ambient = Context::Get()->Ambient();
	lightDesc.Specular = Context::Get()->Specular();
	lightDesc.Direction = Context::Get()->Direction();
	lightDesc.Position = Context::Get()->Position();
}

void PerFrameBuffer::Render()
{
	bufferDesc.View = Context::Get()->View();
	bufferDesc.Projection = Context::Get()->Projection();
	bufferDesc.VP = bufferDesc.View * bufferDesc.Projection;
	D3DXMatrixInverse(&bufferDesc.ViewInverse, nullptr, &bufferDesc.View);

	buffer->Render();
	lightBuffer->Render();
}


// ----------------------------------------------------------------------------
// PerFrame
// ----------------------------------------------------------------------------

PerFrame::PerFrame(Shader * shader)
{
	buffer = new PerFrameBuffer();
	sBuffer = shader->AsConstantBuffer(CB_PerFrame);
	sLightBuffer = shader->AsConstantBuffer(CB_Light);
}

PerFrame::~PerFrame()
{
	SafeDelete(buffer);
}

void PerFrame::Update()
{
	buffer->Update();
}

void PerFrame::Render()
{
	buffer->Render();

	sBuffer->SetConstantBuffer(buffer->BufferPerFrame()->Buffer());
	sLightBuffer->SetConstantBuffer(buffer->BufferLight()->Buffer());
}