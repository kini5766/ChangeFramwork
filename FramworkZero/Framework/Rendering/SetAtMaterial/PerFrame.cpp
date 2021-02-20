#include "Framework.h"
#include "PerFrame.h"


// ----------------------------------------------------------------------------
// PerFrameBuffer
// ----------------------------------------------------------------------------

PerFrame::PerFrame()
{
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	lightBuffer = new ConstantBuffer(Lighting::Get()->DirectionalDesc(), sizeof(DirectionalLightDesc));
	pointLightBuffer = new ConstantBuffer(Lighting::Get()->PointDesc(), sizeof(PointLightsDesc));
	spotLightBuffer = new ConstantBuffer(Lighting::Get()->SpotDesc(), sizeof(SpotLightsDesc));
	burntLightBuffer = new ConstantBuffer(Lighting::Get()->BurntDesc(), sizeof(BurntLightDesc));
}

PerFrame::~PerFrame()
{
	SafeDelete(buffer);
	SafeDelete(lightBuffer);
	SafeDelete(pointLightBuffer);
	SafeDelete(spotLightBuffer);
	SafeDelete(burntLightBuffer);
}

void PerFrame::Update()
{
	desc.Time = Time::Get()->Running();
}

void PerFrame::Render()
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

using namespace ShaderEffectName;
void PerFrame::SetAtMaterial(ShaderSetter * target)
{
	target->SetConstantBuffer(CB_PERFRAME, buffer->Buffer());
	target->SetConstantBuffer(CB_LIGHT, lightBuffer->Buffer());
	target->SetConstantBuffer(CB_LIGHT_POINT, pointLightBuffer->Buffer());
	target->SetConstantBuffer(CB_LIGHT_SPOT, spotLightBuffer->Buffer());
	target->SetConstantBuffer(CB_LIGHT_BURNT, burntLightBuffer->Buffer());
}
