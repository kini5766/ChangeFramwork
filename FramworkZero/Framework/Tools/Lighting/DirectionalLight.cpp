#include "Framework.h"
#include "DirectionalLight.h"

#include "Tools/SphereCoord.h"

DirectionalLight::DirectionalLight(DirectionalLightDesc* desc)
	: desc(desc)
{
	transform = new Transform();
	ApplyLight();
}

DirectionalLight::~DirectionalLight()
{
	SafeDelete(transform);
}

void DirectionalLight::ApplyLight()
{
	SphereCoord scoord;
	scoord.SetRectCoord_Y(desc->Direction);
	Vector3 rotation = scoord.YawPitch_Z();
	transform->Rotation(rotation);
}

void DirectionalLight::ApplyTransform()
{
	desc->Direction = transform->Forward();
}
