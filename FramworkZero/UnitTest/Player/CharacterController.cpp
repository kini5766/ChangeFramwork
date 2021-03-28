#include "stdafx.h"
#include "CharacterController.h"

#include "UserKeyState.h"
#include "Component/RotateSystem.h"
#include "Component/AttackAnimation.h"

CharacterController::CharacterController(Transform * transform, Animator * animator)
	: transform(transform), animator(animator)
{
	userInput = new UserKeyState();
	ratate = new RotateSystem();

	animator->AddFuncChange(4, bind(&CharacterController::Failed, this));
}

CharacterController::~CharacterController()
{
	SafeDelete(ratate);
	SafeDelete(userInput);
}

void CharacterController::Update()
{
	if (currState == 4 &&
		nextState == 4)
	{
		return;
	}

	if (nextState == 4)
	{
		animator->Play(4);
		currState = 4;
		return;
	}

	userInput->Update();

	if (attack->IsAttackAble())
	{
		if (nextState == 3)
			nextState = 0;
		if (Input::Mouse()->Down(0))
		{
			attack->Play();
			nextState = (3);
		}
	}

	if (nextState != 3)
	{
		Vector2 keyAxis = userInput->GetAxis();
		float speedDelta = Time::Delta() * speed;

		if (keyAxis.x == 0 && keyAxis.y == 0)
			nextState = 0;
		else
		{
			//Debug::Gizmo->SetTransform(transform);

			nextState = 1;
			Vector3 up = transform->Up();
			CameraTransform* camera = Context::Get()->MainCamera()->GetTransform();

			Vector3 cameraFoword = camera->Forward();
			Vector3 cameraRight = camera->Right();

			Vector3 rUp = up * D3DXVec3Dot(&up, &cameraRight);
			Vector3 fUp = up * D3DXVec3Dot(&up, &cameraFoword);

			cameraFoword = cameraFoword - fUp;
			cameraRight = cameraRight - rUp;

			D3DXVec3Normalize(&cameraFoword, &cameraFoword);
			D3DXVec3Normalize(&cameraRight, &cameraRight);

			// 이동 방향 얻기
			Vector3 moveAxis = Vector3(0.0f, 0.0f, 0.0f);
			moveAxis += cameraFoword * keyAxis.y;
			moveAxis += cameraRight * keyAxis.x;

			// 회전
			ratate->SetDirection(moveAxis);
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
	}


	if (nextState != currState)
	{
		animator->Play(nextState);
		currState = nextState;
	}

}

void CharacterController::Fall()
{
	nextState = 4;
}

void CharacterController::Riseup()
{
	nextState = 0;
}

void CharacterController::SetFuncAttack(AttackAnimation * value)
{
	attack = value;
}

void CharacterController::Focus(Vector3 * outPosition)
{
	transform->Position(outPosition);
	(*outPosition).y += 3.0f;
}

void CharacterController::Failed()
{
	funcLost();
}

