#include "Framework.h"
#include "OrbitCamera.h"

#include "IFocus.h"
#include "Tools/SphereCoord.h"

OrbitCamera::OrbitCamera()
{
	perspective = new Perspective(Screen::Width(), Screen::Height());
	projection = perspective;
	viewport = new Viewport(Screen::Width(), Screen::Height());


	sphereCoord = new SphereCoord();
	sphereCoord->Rho(distance);
	//sphereCoord->Theta(Math::PI * 0.65f);
	Position(sphereCoord->RectCoord_Y());
	Rotation(sphereCoord->YawPitch_Z());
}

OrbitCamera::~OrbitCamera()
{
	SafeDelete(perspective);
	SafeDelete(viewport);
}

void OrbitCamera::Update()
{
	Vector3 pos;
	Position(&pos);

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
		if (distance < 6.0f)
			distance = 6.0f;
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

	// 충돌 검사
	{
		CollisionManager::Get()->GetColliders(&colliders);

		float d = distance;
		for (Collider* collider : colliders)
		{
			if ((collider->GetMask() & Collider::COLLIDER_LAYER_CAMERA) != 0)
			{
				float temp;
				if (collider->Intersection(focus, -sphereCoord->RectCoord_YNormal(), &temp))
				{
					if (d > temp && temp != 0)
						d = temp;
				}
			}
		}
		sphereCoord->Rho(d);
	}

	pos = focus - sphereCoord->RectCoord_Y();
	Position(pos);

	Rotation(sphereCoord->YawPitch_Z());
}

void OrbitCamera::ResizeScreen(float width, float height)
{
	viewport->Set(width, height);
	perspective->Set(width, height);
}

void OrbitCamera::SetTarget(IFocus * value)
{
	target = value;
}
