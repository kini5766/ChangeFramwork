#include "Framework.h"
#include "PerTransform.h"

using namespace ShaderEffectName;

PerTransform::PerTransform(Shader * _shader)
	: shader(new ShaderSetter(_shader))
{
	perFrame = new PerFrameBuffer();
	shader->SetConstantBuffer(CB_PERFRAME, perFrame->BufferPerFrame()->Buffer());
	shader->SetConstantBuffer(CB_LIGHT, perFrame->LightBuffer()->Buffer());
	shader->SetConstantBuffer(CB_LIGHT_POINT, perFrame->PointLightBuffer()->Buffer());
	shader->SetConstantBuffer(CB_LIGHT_SPOT, perFrame->SpotLightBuffer()->Buffer());
	shader->SetConstantBuffer(CB_LIGHT_BURNT, perFrame->BurntLightBuffer()->Buffer());

	transform = new Transform(&world);
	worldBuffer = new ConstantBuffer(&world, sizeof(Matrix));
	shader->SetConstantBuffer(CB_WORLD, worldBuffer->Buffer());
}

PerTransform::~PerTransform()
{
	SafeDelete(worldBuffer);
	SafeDelete(transform);
	SafeDelete(perFrame);
}

void PerTransform::Update()
{
	perFrame->Update();
}

void PerTransform::Render()
{
	perFrame->Render();
	transform->UpdateWorld();
	worldBuffer->Render();
	shader->Render();
}
