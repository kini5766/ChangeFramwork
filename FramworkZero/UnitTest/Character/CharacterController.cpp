#include "stdafx.h"
#include "CharacterController.h"

#include "UserKeyState.h"

CharacterController::CharacterController(Transform * transform, Animator * animator)
	: transform(transform), animator(animator)
{
	userInput = new UserKeyState();
}

CharacterController::~CharacterController()
{
	SafeDelete(userInput);
}

void CharacterController::Update()
{
	UINT next = currState;
	userInput->Update();

	Vector2 keyAxis;

	keyAxis = userInput->GetAxis();

	if (keyAxis.x == 0 && keyAxis.y == 0)
		next = 0;
	else
	{
		next = 1;
		Vector3 up = transform->Up();
		Vector3 cameraFoword = Context::Get()->MainCamera()->Forward();
		Vector3 cameraRight = Context::Get()->MainCamera()->Right();
		cameraFoword.y = -(up.x * cameraFoword.x + up.z * cameraFoword.y) / up.y;
		cameraRight.y = -(up.x * cameraRight.x + up.z * cameraRight.y) / up.y;
		D3DXVec3Normalize(&cameraFoword, &cameraFoword);
		D3DXVec3Normalize(&cameraRight, &cameraRight);

		Vector3 moveAxis = Vector3(0.0f, 0.0f, 0.0f);
		moveAxis += cameraFoword * keyAxis.y;
		moveAxis += cameraRight * keyAxis.x;

		Vector3 position;
		transform->Position(&position);
		position += moveAxis * Time::Delta() * speed;
		transform->Position(position);
	}

	if (next != currState)
	{
		animator->Play(next);
		currState = next;
	}
}

void CharacterController::Focus(Vector3 * outPosition)
{
	transform->Position(outPosition);
	(*outPosition).y += 3.0f;
}
