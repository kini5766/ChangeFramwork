#include "stdafx.h"
#include "CharacterController.h"

#include "UserKeyState.h"
#include "Component/RotateSystem.h"

CharacterController::CharacterController(Transform * transform, Animator * animator)
	: transform(transform), animator(animator)
{
	userInput = new UserKeyState();
	ratate = new RotateSystem();
}

CharacterController::~CharacterController()
{
	SafeDelete(ratate);
	SafeDelete(userInput);
}

void CharacterController::Update()
{
	UINT next = currState;
	userInput->Update();

	Vector2 keyAxis = userInput->GetAxis();
	float speedDelta = Time::Delta() * speed;

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

		// 이동 방향 얻기
		Vector3 moveAxis = Vector3(0.0f, 0.0f, 0.0f);
		moveAxis += cameraFoword * keyAxis.y;
		moveAxis += cameraRight * keyAxis.x;

		// 회전
		ratate->SetTarget(moveAxis);
		Quaternion q;
		transform->Rotation(&q);
		q = ratate->GetRotation(q, -transform->Forward(), speedDelta);
		transform->Rotation(q);

		// 이동
		Vector3 position;
		transform->Position(&position);
		position += moveAxis * speedDelta;
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
