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

	animator->SetFuncNext(bind(&CharacterController::ClipEnd, this, placeholders::_1));
}

CharacterController::~CharacterController()
{
	SafeDelete(ratate);
	SafeDelete(userInput);
}

void CharacterController::Update()
{
	Debug::Gizmo->SetTransform(transform);

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
			nextState = 1;
			Vector3 up = transform->Up();
			CameraTransform* camera = Context::Get()->MainCamera()->GetTransform();
			Vector3 cameraFoword = camera->Forward();
			Vector3 cameraRight = camera->Right();
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

void CharacterController::ClipEnd(UINT clip)
{
	if (currState == 4)
	{
		if (bDelay)
		{
			funcLost();
			bDelay = false;
		}
		else bDelay = true;
	}
}
