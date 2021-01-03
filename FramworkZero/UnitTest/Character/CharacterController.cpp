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

	Debug::Box->RenderBox(collider->GetTransform(), Color(0.0f, 1.0f, 0.0f, 1.0f));

	receiver->Update();
	for (BoxReceveDesc& m : receiver->GetReceived())
	{
		if (m.Tag == L"1 hit")
		{
			Debug::Log->Print("1 hit!");
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
