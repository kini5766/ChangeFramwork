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

	// move
	{
		Vector3 P;
		Position(&P);

		float moveDelta = move * Time::Delta();
		// move
		if (Input::Keyboard()->Press('W'))
			P += Forward() * moveDelta;
		else if (Input::Keyboard()->Press('S'))
			P -= Forward() * moveDelta;

		if (Input::Keyboard()->Press('D'))
			P += Right() * moveDelta;
		else if (Input::Keyboard()->Press('A'))
			P -= Right() * moveDelta;


		if (Input::Keyboard()->Press('E'))
			P += Up() * moveDelta;
		else if (Input::Keyboard()->Press('Q'))
			P -= Up() * moveDelta;

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
