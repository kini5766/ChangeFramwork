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
			Vector3 cameraFoword = Context::Get()->MainCamera()->Forward();
			Vector3 cameraRight = Context::Get()->MainCamera()->Right();
			cameraFoword.y = -(up.x * cameraFoword.x + up.z * cameraFoword.y) / up.y;
			cameraRight.y = -(up.x * cameraRight.x + up.z * cameraRight.y) / up.y;
			D3DXVec3Normalize(&cameraFoword, &cameraFoword);
			D3DXVec3Normalize(&cameraRight, &cameraRight);

			// �̵� ���� ���
			Vector3 moveAxis = Vector3(0.0f, 0.0f, 0.0f);
			moveAxis += cameraFoword * keyAxis.y;
			moveAxis += cameraRight * keyAxis.x;

			// ȸ��
			ratate->SetTarget(moveAxis);
			Quaternion q;
			transform->Rotation(&q);
			q = ratate->GetRotation(q, -transform->Forward(), speedDelta);
			transform->Rotation(q);

			// �̵�
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
	//if (currState == 3 && bDelay)
	//{
	//	nextState = clip;
	//	bDelay = false;
	//}
	//if (clip == 3)
	//{
	//	bDelay = true;
	//}
}