#include "stdafx.h"
#include "Patrolling.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/PerceptionSystem.h"


Patrolling::Patrolling(const PatrollingDesc & input)
	: desc(input)
	, funcReset(bind(&Patrolling::Reset, this))
{
	FlowTesk::FuncCall = bind(&Patrolling::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&Patrolling::Update, this);
	FlowTesk::FuncCancel = bind(&Patrolling::Cancel, this);

	waiter = new Waiter(desc.MakeWaiter());

	UINT size = desc.PatrolCount;
	pats.reserve(size);
	for (int i = size - 1; i >= 0; i--)
		pats.push_back(new PointMover(desc.MakeMover(i)));

	reader = new FlowReader();


	for (PointMover* pat : pats)
	{
		// 순서
		// 2. 대기
		// 1. 이동

		tesks.push_back(waiter);
		tesks.push_back(pat);
	}
}

Patrolling::~Patrolling()
{
	SafeDelete(reader);

	for (auto d : pats)
		SafeDelete(d);
	SafeDelete(waiter);
}

void Patrolling::Reset()
{
	reader->Clear();
	if (tesks.size() == 0)
		return;

	reader->PushBacks(tesks.size(), tesks.data());
	reader->Call(&funcReset);
}


void Patrolling::Call(const FutureAction * action)
{
	result.SetAction(action);
	reader->Call(&funcReset);
}

void Patrolling::Update()
{
	desc.Perceptor->Update();
	if (desc.Perceptor->IsPerceived())
	{
		desc.FuncInRange();
		result.OnAction();
		return;
	}

	reader->Update();
}

void Patrolling::Cancel()
{
	result.Clear();
	reader->Cancel();
}
