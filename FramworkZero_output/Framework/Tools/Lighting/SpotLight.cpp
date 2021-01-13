#include "Framework.h"
#include "SpotLight.h"

#include "Tools/SphereCoord.h"

SpotLight::SpotLight(SpotLightDesc* desc)
	: desc(desc)
{
	transform = new Transform();
	ApplyLight();
}

SpotLight::~SpotLight()
{
	SafeDelete(transform);
}

void SpotLight::ApplyLight()
{
	transform->Position(desc->Position);

	SphereCoord scoord;
	scoord.SetRectCoord_Y(transform->Forward());
	Vector3 rotation = scoord.YawPitch_Z();
	transform->Rotation(rotation);

	Vector3 s;
	transform->Scale(&s);
	s.z = desc->Range;
	s.x = desc->Angle;
	transform->Scale(s);
}

void SpotLight::ApplyTransform()
{
	transform->Position(&desc->Position);
	desc->Direction = transform->Forward();

	Vector3 s;
	transform->Scale(&s);
	desc->Range = s.z;
	desc->Angle = s.x;
}
