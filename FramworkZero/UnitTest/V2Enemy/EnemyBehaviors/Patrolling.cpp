#include "stdafx.h"
#include "Patrolling.h"

#include "V2Enemy/EnemyDetectionSystem.h"
#include "EB_Pieces/LookAround.h"
#include "EB_Pieces/Patrol.h"

Patrolling::Patrolling(const PatrollingDesc & desc)
	: desc(desc)
	, funcReset(bind(&Patrolling::Reset, this))
{
	reader = new DelayReader();
	lookAr = new LookAround(this->desc);

	pats.reserve(desc.PatrolCount);
	for (UINT i = 0; i < desc.PatrolCount; i++)
		pats.push_back(new Patrol(this->desc.CastPatrol(i)));
}

Patrolling::~Patrolling()
{
	for (auto d : pats)
		SafeDelete(d);
	SafeDelete(lookAr);
	SafeDelete(reader);
}

void Patrolling::Reset()
{
	reader->Clear();
	for (Patrol* pat : pats)
	{
		reader->PushBack(pat);
		reader->PushBack(lookAr);
	}
	reader->Call(&funcReset);
}


void Patrolling::Call(const ReturnAction * action)
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
