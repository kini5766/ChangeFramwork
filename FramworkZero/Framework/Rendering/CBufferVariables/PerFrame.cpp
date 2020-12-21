#include "Framework.h"
#include "PerFrame.h"

using namespace ShaderEffectName;

// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

PerFrameBuffer::PerFrameBuffer()
{
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	lightBuffer = new ConstantBuffer(&lightDesc, sizeof(LightDesc));
}

PerFrameBuffer::~PerFrameBuffer()
{
	SafeDelete(buffer);
	SafeDelete(lightBuffer);
}

void PerFrameBuffer::Update()
{
	desc.View = Context::Get()->View();
	D3DXMatrixInverse(&desc.ViewInverse, nullptr, &desc.View);
	desc.Projection = Context::Get()->Projection();
	desc.VP = desc.View * desc.Projection;
	//desc.Culling
	//desc.Clipping
	desc.Time = Time::Get()->Running();

	lightDesc.Ambient = Lighting::Get()->Ambient();
	lightDesc.Specular = Lighting::Get()->Specular();
	lightDesc.Direction = Lighting::Get()->Direction();
	lightDesc.Position = Lighting::Get()->Position();
}

void PerFrameBuffer::Render()
{
	buffer->Render();
	lightBuffer->Render();
}


// ----------------------------------------------------------------------------
// PerFrame
// ----------------------------------------------------------------------------

PerFrame::PerFrame(Shader * shader)
	: shader(new ShaderSetter(shader))
{
	buffer = new PerFrameBuffer();
	this->shader->SetConstantBuffer(CB_PerFrame, buffer->BufferPerFrame()->Buffer());
	this->shader->SetConstantBuffer(CB_Light, buffer->LightBuffer()->Buffer());
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
	shader->Render();
}
