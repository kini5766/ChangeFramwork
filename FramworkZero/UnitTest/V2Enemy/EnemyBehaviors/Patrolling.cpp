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

	clipLookAround = new ClipPlayer(this->desc.MakeLookAround());
	waiter = new Waiter(this->desc.MakeWaiter());
	clipWalk = new ClipPlayer(this->desc.MakeWalk());

	pats.reserve(desc.PatrolCount);
	for (UINT i = 0; i < desc.PatrolCount; i++)
		pats.push_back(new PointMover(this->desc.MakeMover(i)));
}

Patrolling::~Patrolling()
{
	for (auto d : pats)
		SafeDelete(d);

	SafeDelete(clipWalk);
	SafeDelete(waiter);
	SafeDelete(clipLookAround);

	SafeDelete(reader);
}

void Patrolling::Reset()
{
	reader->Clear();
	for (PointMover* pat : pats)
	{
		// 순서
		// 4. 대기
		// 3. 둘러보기(애니매이션)
		// 2. 이동
		// 1. 걷기(애니매이션)

		reader->PushBack(waiter);
		reader->PushBack(clipLookAround);
		reader->PushBack(pat);
		reader->PushBack(clipWalk);
	}
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
