#include "stdafx.h"
#include "CharacterController.h"

#include "UserKeyState.h"

CharacterController::CharacterController(Transform * transform, Animator * animator)
	: transform(transform), animator(animator)
{
	userInput = new UserKeyState();
	collider = CollisionManager::Get()->CreateCollider();
	collider->GetTransform()->SetParent(transform);
	Matrix w;
	D3DXMatrixIdentity(&w);
	collider->GetTransform()->Position(0.0f, 100.0f, 0.0f);
	collider->GetTransform()->Rotation(0.0f, 0.0f, 0.0f);
	collider->GetTransform()->Scale(75.0f, 200.0f, 75.0f);
	collider->SetLayer(COLLIDER_LAYER_HITBOX);

	receiver = new ReceiveBox(collider);
	receiver->AddReceiveTag(L"1 hit");
	receiver->AddReceiveTag(L"update hit");
	receiver->AddReceiveTag(L"1 second hit");
}

CharacterController::~CharacterController()
{
	SafeDelete(receiver);
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

		Vector3 moveAxis = Vector3(0.0f, 0.0f, 0.0f);
		moveAxis += cameraFoword * keyAxis.y;
		moveAxis += cameraRight * keyAxis.x;
		//
		{
			Vector3 forward = -transform->Forward();

			float dDot = D3DXVec3Dot(&moveAxis, &forward);
			// 남은 방향
			float rad = acosf(dDot);

			// 남은 방향이 없을 경우
			if (rad <= FLT_EPSILON) {}
			else if (rad < speedDelta)
			{
				// 남은 방향(축)
				Vector3 dAxis;
				D3DXVec3Cross(&dAxis, &forward, &moveAxis);
				D3DXVec3Normalize(&dAxis, &dAxis);

				// 목표 회전
				Quaternion dQ;
				D3DXQuaternionRotationAxis(&dQ, &dAxis, rad);

				Quaternion q;
				transform->Rotation(&q);
				transform->Rotation(q * dQ);
			}
			else
			{
				// 남은 방향(축)
				Vector3 dAxis;
				D3DXVec3Cross(&dAxis, &forward, &moveAxis);
				D3DXVec3Normalize(&dAxis, &dAxis);

				// speedDelta만큼 회전
				Quaternion dQ;
				D3DXQuaternionRotationAxis(&dQ, &dAxis, speedDelta);

				Quaternion q;
				transform->Rotation(&q);
				transform->Rotation(q * dQ);
			}
		}
		//
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

	Debug::Box->RenderBox(collider->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));

	receiver->Update();
	for (BoxReceveDesc& m : receiver->GetReceived())
	{
		if (m.Tag == L"1 hit")
		{
			Debug::Log->Print("1 hit!");
		}
		else if (m.Tag == L"1 second hit")
		{
			Debug::Log->Print("1 second hit!");
		}
		else if (m.Tag == L"update hit")
		{
			Debug::Log->Show("--<update hit!>--");
		}
	}
}

void CharacterController::Focus(Vector3 * outPosition)
{
	transform->Position(outPosition);
	(*outPosition).y += 3.0f;
}
