#include "stdafx.h"
#include "BrushInput.h"

BrushInput::BrushInput()
{
}

BrushInput::~BrushInput()
{
}

void BrushInput::Update()
{
	state = MouseState::NONE;
	bMouseMove = false;

	// bMouseMove
	mouse = Input::Mouse()->GetPosition();
	if (mouse != prevMousePos)
	{
		prevMousePos = mouse;
		bMouseMove = true;
	}

	// state
	if (
		ImGui::GetIO().WantCaptureMouse &&
		ImGui::IsAnyItemHovered()// &&
		// ImGui::IsAnyWindowHovered()
		)
	{
		return;
	}

	if (Input::Mouse()->Down(0))
	{
		state = MouseState::DOWN;
	}
	else if (Input::Mouse()->Press(0))
	{
		state = MouseState::PRESS;
	}
	else if (Input::Mouse()->Up(0))
	{
		state = MouseState::UP;
	}
	else
	{
		if (Input::Keyboard()->Down('Z'))
		{
			state = MouseState::PICK;
		}
	}

}
