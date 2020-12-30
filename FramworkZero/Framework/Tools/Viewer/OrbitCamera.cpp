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
	sphereCoord->Rho(15.0f);
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

	// ���콺 ��ǲ
	Vector3 axis = Input::Mouse()->GetMoveValue();

	// Ȯ�� ���
	{
		float rho = sphereCoord->Rho();
		rho -= axis.z * 0.007f;
		if (rho < 6.0f)
			rho = 6.0f;
		else if (rho > 100.0f)
			rho = 100.0f;
		sphereCoord->Rho(rho);
	}

	if (Input::Mouse()->Press(1))
	{
		// ȸ��
		float theta = sphereCoord->Theta();
		theta += axis.y * rotateSpeed;
		sphereCoord->Theta(theta);

		float phi = sphereCoord->Phi();
		phi += axis.x * rotateSpeed;
		sphereCoord->Phi(phi);
	}

	// �ִ� �ּ� ȸ��
	float theta = sphereCoord->Theta();
	if (theta < Math::PI * 0.15f)
		theta = Math::PI * 0.15f;
	if (theta > Math::PI * 0.9f)
		theta = Math::PI * 0.9f;
	sphereCoord->Theta(theta);

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
