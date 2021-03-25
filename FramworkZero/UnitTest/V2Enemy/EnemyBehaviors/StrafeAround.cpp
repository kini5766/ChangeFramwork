#include "stdafx.h"
#include "StrafeAround.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "Component/PointMoveSystem.h"
#include "V2Enemy/PerceptionSystem.h"


StrafeAround::StrafeAround(const StrafeAroundDesc & input)
	: desc(input)
	, funcReset(bind(&StrafeAround::Reset, this))
{
	reader = new FlowReader();


	wait = new FlowRoutine();
	playIdle = new FlowAction(PlayIdle());
	waiter = new Waiter(desc.MakeWaiter());

	playRun = new FlowAction(PlayRun());
	follow = new FlowRoutine();
	follower = new Follower(desc.MakeFollower());
	around = new FlowRoutine();
	moveto = new PointMover(desc.MakeMover());

	// ����
	// 2. ���
	// 1. �ִϸ��̼� ���
	wait->Tesks()->push_back(waiter);
	wait->Tesks()->push_back(playIdle);

	// ����
	// 2. ����
	// 1. ���
	follow->Tesks()->push_back(follower);
	follow->Tesks()->push_back(playRun);

	// ����
	// 2. ��ȸ
	// 1. ���
	around->Tesks()->push_back(moveto);
	around->Tesks()->push_back(playRun);

}

StrafeAround::~StrafeAround()
{
	SafeDelete(reader);

	SafeDelete(wait);
	SafeDelete(playIdle);
	SafeDelete(waiter);

	SafeDelete(playRun);
	SafeDelete(follow);
	SafeDelete(follower);
	SafeDelete(around);
	SafeDelete(moveto);
}

void StrafeAround::Call(const FutureAction * action)
{
	result.SetAction(action);

	curr = desc.Perceptor->GetDistanceSq() <= desc.ApproachRangeSq;
	Reset();
}

void StrafeAround::Update()
{
	desc.Perceptor->Update();
	if (desc.Perceptor->IsPerceived() == false)
	{
		desc.FuncOutRange();
		result.OnAction();
		return;
	}

	bool next = desc.Perceptor->GetDistanceSq() <= desc.ApproachRangeSq;
	if (next != curr)
	{
		curr = next;
		Reset();
	}

	reader->Update();
}

void StrafeAround::Cancel()
{
	result.Clear();
	reader->Cancel();
}

void StrafeAround::Reset()
{
	reader->Cancel();
	if (curr)
	{
		// �ֺ� �ɵ���
		Vector3 d = desc.Perceptor->GetDest();

		Vector3 dc(0, 1, 0);
		D3DXVec3Cross(&dc, &d, &dc);


		// -45�� ~ 45��
		float r = Math::Random(-Math::PI * 0.25f, Math::PI * 0.25f);
		//float r = Math::Random(Math::PI * 0.16f, Math::PI * 0.32f);
		Vector2 axis;
		axis.x = -cosf(r);
		axis.y = -sinf(r);

		Vector3 forward = d * axis.x;
		Vector3 right = dc * axis.y;
		Vector3 direction = forward + right;

		D3DXVec3Normalize(&direction, &direction);
		direction *= Math::Random(desc.MinRange, desc.MinRange * 2.0f);

		Vector3 position = *desc.Perceptor->GetFocus();
		moveto->GetDesc()->Point = position + direction;

		waiter->GetDesc()->Time = Math::Random(0.25f, 1.0f);

		// ����
		// 2. ���
		// 1. �̵�
		//reader->PushBack(waiter);
		reader->PushBack(around);
	}
	else
	{
		// ���󰡱�
		reader->PushBack(follow);
	}

	reader->Call(&funcReset);
}

FutureAction StrafeAround::PlayIdle()
{
	return [=]() {
		desc.Anim->PlayUpdate(desc.ClipIdle);
	};
}

FutureAction StrafeAround::PlayRun()
{
	return [=]() {
		desc.Anim->PlayUpdate(desc.ClipRun);
		desc.MovingSystem->SetMoveSpeeder(desc.RunSpeed);
	};
}
