#include "stdafx.h"
#include "UserKeyState.h"

UserKeyState::UserKeyState()
{
}

UserKeyState::~UserKeyState()
{
}

void UserKeyState::Update()
{
	axis = Vector2(0.0f, 0.0f);
	if (Input::Keyboard()->Press(VK_DOWN) ||
		Input::Keyboard()->Press('S'))
		axis.y -= 1.0f;
	if (Input::Keyboard()->Press(VK_UP) ||
		Input::Keyboard()->Press('W'))
		axis.y += 1.0f;
	if (Input::Keyboard()->Press(VK_LEFT) ||
		Input::Keyboard()->Press('A'))
		axis.x -= 1.0f;
	if (Input::Keyboard()->Press(VK_RIGHT) ||
		Input::Keyboard()->Press('D'))
		axis.x += 1.0f;
}
