#include "stdafx.h"
#include "Patrolling.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/EnemyDetectionSystem.h"


Patrolling::Patrolling(const PatrollingDesc & desc)
	: desc(desc)
	, funcReset(bind(&Patrolling::Reset, this))
{
	FlowTesk::FuncCall = bind(&Patrolling::Call, this, placeholders::_1);
	FlowTesk::FuncUpdate = bind(&Patrolling::Update, this);
	FlowTesk::FuncCancel = bind(&Patrolling::Cancel, this);

	reader = new FlowReader();

	waiter = new Waiter(this->desc.MakeWaiter());

	UINT size = desc.PatrolCount;
	pats.reserve(size);
	for (int i = size - 1; i >= 0; i--)
		pats.push_back(new PointMover(this->desc.MakeMover(i)));


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
	for (auto d : pats)
		SafeDelete(d);
	SafeDelete(waiter);

	SafeDelete(reader);
}

void Patrolling::Reset()
{
	if (tesks.size() == 0)
		return;

	reader->Clear();
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
	if (desc.DetectionSystem->IsInRange())
	{
		desc.DetectionSystem->OnInRange();
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
