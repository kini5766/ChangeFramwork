#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
{
	perspective = new Perspective(Screen::Width(), Screen::Height());
	projection = perspective;
	viewport = new Viewport(Screen::Width(), Screen::Height());
}

Freedom::~Freedom()
{
	SafeDelete(perspective);
	SafeDelete(viewport);
}

void Freedom::Update()
{
	if (Input::Mouse()->Press(1) == false)
		return;

	Vector3 f = Forward();
	Vector3 r = Right();
	Vector3 u = Up();

	// move
	{
		Vector3 P;
		Position(&P);

		if (Input::Keyboard()->Press('W'))
			P = P + f * move * Time::Delta();
		else if (Input::Keyboard()->Press('S'))
			P = P - f * move * Time::Delta();

		if (Input::Keyboard()->Press('D'))
			P = P + r * move * Time::Delta();
		else if (Input::Keyboard()->Press('A'))
			P = P - r * move * Time::Delta();


		if (Input::Keyboard()->Press('E'))
			P = P + u * move * Time::Delta();
		else if (Input::Keyboard()->Press('Q'))
			P = P - u * move * Time::Delta();

		Position(P);
	}

	// roation
	{
		Vector3 axis = Input::Mouse()->GetMoveValue();
		RotatePitch(axis.y * rotation);
		RotateYaw(axis.x * rotation);
	}
}

void Freedom::ResizeScreen(float width, float height)
{
	viewport->Set(width, height);
	perspective->Set(width, height);
}

void Freedom::Speed(float _move, float _rotation)
{
	move = _move; rotation = _rotation;
}
