#include "Framework.h"
#include "PerFrame.h"

using namespace ShaderEffectName;

// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

PerFrameBuffer::PerFrameBuffer()
{
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	lightBuffer = new ConstantBuffer(Lighting::Get()->DirectionalDesc(), sizeof(DirectionalLightDesc));
	pointLightBuffer = new ConstantBuffer(Lighting::Get()->PointDesc(), sizeof(PointLightsDesc));
	spotLightBuffer = new ConstantBuffer(Lighting::Get()->SpotDesc(), sizeof(SpotLightsDesc));
}

PerFrameBuffer::~PerFrameBuffer()
{
	SafeDelete(buffer);
	SafeDelete(lightBuffer);
	SafeDelete(pointLightBuffer);
	SafeDelete(spotLightBuffer);
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
}

void PerFrameBuffer::Render()
{
	buffer->Render();
	lightBuffer->Render();
	pointLightBuffer->Render();
	spotLightBuffer->Render();
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
	this->shader->SetConstantBuffer(CB_PointLights, buffer->PointLightBuffer()->Buffer());
	this->shader->SetConstantBuffer(CB_SpotLights, buffer->SpotLightBuffer()->Buffer());
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
