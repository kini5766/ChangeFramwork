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
	burntLightBuffer = new ConstantBuffer(Lighting::Get()->BurntDesc(), sizeof(BurntLightDesc));
}

PerFrameBuffer::~PerFrameBuffer()
{
	SafeDelete(buffer);
	SafeDelete(lightBuffer);
	SafeDelete(pointLightBuffer);
	SafeDelete(spotLightBuffer);
	SafeDelete(burntLightBuffer);
}

void PerFrameBuffer::Update()
{
	desc.Time = Time::Get()->Running();
}

void PerFrameBuffer::Render()
{
	desc.View = Context::Get()->View();
	D3DXMatrixInverse(&desc.ViewInverse, nullptr, &desc.View);
	desc.Projection = Context::Get()->Projection();
	desc.VP = desc.View * desc.Projection;

	desc.Clipping = Context::Get()->Clipping();
	memcpy(desc.Culling, Context::Get()->Culling(), sizeof(Plane) * 4);

	buffer->Render();
	lightBuffer->Render();
	pointLightBuffer->Render();
	spotLightBuffer->Render();
	burntLightBuffer->Render();
}


// ----------------------------------------------------------------------------
// PerFrame
// ----------------------------------------------------------------------------

PerFrame::PerFrame(Shader * shader)
	: shader(new ShaderSetter(shader))
{
	buffer = new PerFrameBuffer();
	this->shader->SetConstantBuffer(CB_PERFRAME, buffer->BufferPerFrame()->Buffer());
	this->shader->SetConstantBuffer(CB_LIGHT, buffer->LightBuffer()->Buffer());
	this->shader->SetConstantBuffer(CB_LIGHT_POINT, buffer->PointLightBuffer()->Buffer());
	this->shader->SetConstantBuffer(CB_LIGHT_SPOT, buffer->SpotLightBuffer()->Buffer());
	this->shader->SetConstantBuffer(CB_LIGHT_BURNT, buffer->BurntLightBuffer()->Buffer());
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
