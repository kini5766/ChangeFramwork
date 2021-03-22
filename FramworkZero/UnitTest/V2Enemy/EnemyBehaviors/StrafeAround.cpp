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

	waiter = new Waiter(desc.MakeWaiter());
	follower = new Follower(desc.MakeFollower());
	arounder = new PointMover(desc.MakeMover());
}

StrafeAround::~StrafeAround()
{
	SafeDelete(reader);

	SafeDelete(waiter);
	SafeDelete(follower);
	SafeDelete(arounder);
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
		// 주변 맴돌기
		Vector3 d = desc.Perceptor->GetDest();

		Vector3 dc;
		D3DXVec3Cross(&dc, &d, &Vector3(0, 1, 0));


		// -45도 ~ 45도
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
		arounder->GetDesc()->Point = position + direction;

		waiter->GetDesc()->Time = Math::Random(0.25f, 1.0f);

		// 순서
		// 2. 대기
		// 1. 이동
		reader->PushBack(waiter);
		reader->PushBack(arounder);
	}
	else
	{
		// 따라가기
		reader->PushBack(follower);
	}

	reader->Call(&funcReset);
}
