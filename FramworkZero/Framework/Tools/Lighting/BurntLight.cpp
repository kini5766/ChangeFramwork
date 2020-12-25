#include "Framework.h"
#include "BurntLight.h"

BurntLight::BurntLight(BurntLightDesc * desc)
	: desc(desc)
{
	transform = new Transform();
	ApplyLight();
}

BurntLight::~BurntLight()
{
	SafeDelete(transform);
}

void BurntLight::ApplyLight()
{
	transform->Position(desc->Position);
	transform->Scale(desc->Scale);
}

void BurntLight::ApplyTransform()
{
	transform->Position(&desc->Position);
	transform->Scale(&desc->Scale);
}
