#include "stdafx.h"
#include "Patrolling.h"

#include "V2Enemy/EnemyDetectionSystem.h"
#include "EB_Pieces/LookAround.h"
#include "EB_Pieces/Patrol.h"

Patrolling::Patrolling(const PatrollingDesc & desc)
	: desc(desc)
	, bindedReset(bind(&Patrolling::Reset, this))
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
	reader->Call(&bindedReset);
}


void Patrolling::Call(const DelayReturn * _result)
{
	result = _result;
	reader->Call(&bindedReset);
}

void Patrolling::Update()
{
	if (desc.DetectionSystem->IsInRange())
	{
		desc.DetectionSystem->OnInRange();
		(*result)();
		result = nullptr;
		return;
	}

	reader->Update();
}

void Patrolling::Cancel()
{
	reader->Cancel();
}
