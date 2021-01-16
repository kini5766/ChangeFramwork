#include "Framework.h"
#include "DirectionalLight.h"

#include "Tools/Coord/SphereCoord.h"

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

	transform->Rotation(scoord.YawPitch_Z());
}

void DirectionalLight::ApplyTransform()
{
	desc->Direction = transform->Forward();
}
