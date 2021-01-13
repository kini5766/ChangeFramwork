#include "Framework.h"
#include "PointLight.h"

PointLight::PointLight(PointLightDesc * desc)
	: desc(desc)
{
	transform = new Transform();

	ApplyLight();
}

PointLight::~PointLight()
{
	SafeDelete(transform);
}

void PointLight::ApplyLight()
{
	transform->Position(desc->Position);
	Vector3 s;
	transform->Scale(&s);
	s.z = desc->Range;
	transform->Scale(s);
}

void PointLight::ApplyTransform()
{
	transform->Position(&desc->Position);
	Vector3 s;
	transform->Scale(&s);
	desc->Range = s.z;
}
