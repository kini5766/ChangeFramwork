#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
	: Camera(unique_ptr<Projection>(new Perspective(Screen::Width(), Screen::Height())))
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	if (Input::Mouse()->Press(1) == false)
		return;

	// move
	{
		Vector3 P;
		transform->Position(&P);

		float moveDelta = move * Time::Delta();
		// move
		if (Input::Keyboard()->Press('W'))
			P += transform->Forward() * moveDelta;
		else if (Input::Keyboard()->Press('S'))
			P -= transform->Forward() * moveDelta;

		if (Input::Keyboard()->Press('D'))
			P += transform->Right() * moveDelta;
		else if (Input::Keyboard()->Press('A'))
			P -= transform->Right() * moveDelta;


		if (Input::Keyboard()->Press('E'))
			P += transform->Up() * moveDelta;
		else if (Input::Keyboard()->Press('Q'))
			P -= transform->Up() * moveDelta;

		transform->Position(P);
	}

	// roation
	{
		Vector3 axis = Input::Mouse()->GetMoveValue();

		EulerAngle r = transform->RotationEuler();
		{
			Vector3 result = r.EulerDegree();
			result.x += axis.y * rotation;
			result.y += axis.x * rotation;
			r.SetDegree(result);
		}
		transform->RotationEuler(r);
	}
}

void Freedom::Speed(float _move, float _rotation)
{
	move = _move; rotation = _rotation;
}
