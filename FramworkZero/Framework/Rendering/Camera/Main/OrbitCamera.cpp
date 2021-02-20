#include "Framework.h"
#include "OrbitCamera.h"

#include "IFocus.h"
#include "Tools/Coord/SphereCoord.h"

OrbitCamera::OrbitCamera()
	: Camera(unique_ptr<Projection>(new Perspective(Screen::Width(), Screen::Height())))
{
	sphereCoord = new SphereCoord();
	sphereCoord->Rho(distance);
	//sphereCoord->Theta(Math::PI * 0.65f);
	transform->Position(sphereCoord->RectCoord_Y());
	transform->RotationEuler(sphereCoord->YawPitch_Z());

	ray = CollisionManager::Get()->CreateRaycast(
		Ray(Vector3(0.0f, 0.0f, 0.0f), -sphereCoord->RectCoord_YNormal()),
		COLLIDER_LAYER_CAMERA
	);
}

OrbitCamera::~OrbitCamera()
{
	ray->Destroy();
}

void OrbitCamera::Update()
{
	Vector3 pos;
	transform->Position(&pos);

	Vector3 focus(0.0f, 0.0f, 0.0f);
	if (target != nullptr)
	{
		target->Focus(&focus);
	}

	// look at
	SphereCoord dest;
	dest.SetRectCoord_Y(focus - pos);
	//dest.Theta(sphereCoord->Theta());
	SphereCoord::Lerp(sphereCoord, sphereCoord, &dest, 0.5f);

	// 마우스 인풋
	Vector3 axis = Input::Mouse()->GetMoveValue();

	// 확대 축소
	{
		//float rho = sphereCoord->Rho();
		distance -= axis.z * 0.007f;
		if (distance < 1.0f)
			distance = 1.0f;
		else if (distance > 100.0f)
			distance = 100.0f;
		//sphereCoord->Rho(rho);
	}

	if (Input::Mouse()->Press(1))
	{
		// 회전
		float theta = sphereCoord->Theta();
		theta += axis.y * rotateSpeed;
		sphereCoord->Theta(theta);

		float phi = sphereCoord->Phi();
		phi += axis.x * rotateSpeed;
		sphereCoord->Phi(phi);
	}

	// 최대 최소 회전
	float theta = sphereCoord->Theta();
	if (theta < Math::PI * 0.15f)
		theta = Math::PI * 0.15f;
	if (theta > Math::PI * 0.9f)
		theta = Math::PI * 0.9f;
	sphereCoord->Theta(theta);

	// 콜라이더 충돌
	if (ray->IsCollision())
	{
		float minDistance = min(distance, ray->GetMinDistance());
		if (minDistance != 0.0f)
			sphereCoord->Rho(minDistance);
		else sphereCoord->Rho(distance);
	}
	else
	{
		sphereCoord->Rho(distance);
	}

	pos = focus - sphereCoord->RectCoord_Y();
	transform->Position(pos);

	transform->RotationEuler(sphereCoord->YawPitch_Z());
	ray->SetRay(Ray(focus, -sphereCoord->RectCoord_YNormal()));
}

void OrbitCamera::SetTarget(IFocus * value)
{
	target = value;
}
