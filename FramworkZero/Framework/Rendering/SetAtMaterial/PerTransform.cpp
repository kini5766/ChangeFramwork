#include "Framework.h"
#include "PerTransform.h"


PerTransform::PerTransform()
{
	perFrame = new PerFrame();
	transform = new Transform(&world);
	worldBuffer = new ConstantBuffer(&world, sizeof(Matrix));
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
}


using namespace ShaderEffectName;
void PerTransform::SetAtMaterial(ShaderSetter * target)
{
	perFrame->SetAtMaterial(target);
	target->SetConstantBuffer(CB_WORLD, worldBuffer->Buffer());
}
