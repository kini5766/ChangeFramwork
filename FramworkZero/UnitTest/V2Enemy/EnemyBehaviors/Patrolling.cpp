#include "stdafx.h"
#include "Patrolling.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/PerceptionSystem.h"
#include "Component/PointMoveSystem.h"


Patrolling::Patrolling(const PatrollingDesc & input)
	: desc(input)
{
	// -- Play Animation -- //
	playLookAround = new FlowAction([=]() {
		desc.Anim->PlayUpdate(desc.ClipLookAround);
	});
	playWalk = new FlowAction([=]() {
		desc.Anim->PlayUpdate(desc.ClipWalk);
		desc.MovingSystem->SetMoveSpeeder(desc.WalkSpeed);
	});

	// -- Look Around -- //
	WaiterDesc makeWaiter;
	makeWaiter.Time = desc.LookAroundTime;
	waiter = new Waiter(makeWaiter);

	// -- Patrolls -- //
	UINT size = desc.PatrolCount;
	moveToPoints.reserve(size);
	PointMoverDesc makeMover;
	for (int i = size - 1; i >= 0; i--)
	{
		makeMover.MovingSystem = desc.MovingSystem;
		makeMover.Point = desc.PatrolPoints[i];

		moveToPoints.push_back(new PointMover(makeMover));
	}


	// -- Main Routine -- //
	loop = new FlowLoop();
	lookAround = new FlowRoutine();
	patrolls.reserve(size);

	for (PointMover* mov : moveToPoints)
	{
		// 둘러보기
		// 2. 대기
		// 1. 애니메이션 재생
		lookAround->Tesks()->push_back(waiter);
		lookAround->Tesks()->push_back(playLookAround);

		// 패트롤
		// 2. 이동
		// 1. 애니메이션 재생
		FlowRoutine* pat = new FlowRoutine();
		patrolls.push_back(pat);
		pat->Tesks()->push_back(mov);
		pat->Tesks()->push_back(playWalk);

		// 순서
		// 2. 둘러보기
		// 1. 만약 근처가 아니면 다음지점으로
		loop->Tesks()->push_back(lookAround);
		loop->Tesks()->push_back(pat);
	}

	loop->SetJudgment(UpdateLoop());
}

Patrolling::~Patrolling()
{
	SafeDelete(loop);

	for (auto d : patrolls)
		SafeDelete(d);
	for (auto d : moveToPoints)
		SafeDelete(d);

	SafeDelete(waiter);
	SafeDelete(playWalk);
	SafeDelete(lookAround);
	SafeDelete(playLookAround);
}


void Patrolling::Call(const FutureAction * action)
{
	loop->Call(action);
}

void Patrolling::Update()
{
	loop->Update();
}

void Patrolling::Cancel()
{
	loop->Cancel();
}


Judgment Patrolling::UpdateLoop()
{
	return [=]() {
		desc.Perceptor->Update();
		if (desc.Perceptor->IsPerceived())
		{
			desc.FuncInRange();
			return false;
		}
		return true; 
	};
}


Judgment Patrolling::IsAround()
{
	return [=]() {
		return desc.MovingSystem->IsAround();
	};
}
