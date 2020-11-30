#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
	: Camera()
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	if (Mouse::Get()->Press(1) == false) 
		return;

	Vector3 f = Forward();
	Vector3 u = Up();
	Vector3 r = Right();

	// Move
	{
		Vector3 p;
		Position(&p);

		if (Keyboard::Get()->Press('W')) p = p + f * move * Time::Delta();
		if (Keyboard::Get()->Press('S')) p = p - f * move * Time::Delta();
		if (Keyboard::Get()->Press('D')) p = p + r * move * Time::Delta();
		if (Keyboard::Get()->Press('A')) p = p - r * move * Time::Delta();
		if (Keyboard::Get()->Press('E')) p = p + u * move * Time::Delta();
		if (Keyboard::Get()->Press('Q')) p = p - u * move * Time::Delta();

		Position(p);
	}

	// Rotation
	{
		Vector3 r;
		Rotation(&r);

		Vector3 val = Mouse::Get()->GetMoveValue();
		r.x += val.y * rotation * 0.000314f;
		r.y += val.x * rotation * 0.000314f;
		r.z = 0.0f;

		Rotation(r);
	}
}

void Freedom::Speed(float _move, float _rotation)
{
	move = _move;
	rotation = _rotation;
}
