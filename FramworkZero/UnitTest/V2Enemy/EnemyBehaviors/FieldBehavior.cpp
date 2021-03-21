#include "stdafx.h"
#include "FieldBehavior.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/PerceptionSystem.h"
#include "Component/PointMoveSystem.h"
#include "Patrolling.h"
#include "CombatPosture.h"

FieldBehavior::FieldBehavior(const FieldBehaviorInput & input)
	: desc(input)
	, transform(input.Trans)
	, inRange(bind(&FieldBehavior::InRange, this))
	//, outRange(bind(&FieldBehavior::OutRange, this))
{
	PerceptionInput inputED;
	inputED.MinePosition = &minePosition;
	inputED.FocusPosition = &focusPosition;
	inputED.SightRangeSq = desc.SightRangeSq;
	inputED.LoseRangeSq = desc.LoseRangeSq;

	desc.Perceptor = new PerceptionSystem(inputED);
	desc.FuncInRange = inRange;
	//desc.FuncOutRange = outRange;

	pat = new Patrolling(desc);
	clipInSight = new ClipOncePlayer(desc.MakeInSight());
	combat = new CombatPosture(desc.MakeCombat());
	recall = new PointMover(desc.MakeComeback());

	reader = new FlowReader();
	reader->PushBack(pat);
	reader->Call();
}

FieldBehavior::~FieldBehavior()
{
	SafeDelete(reader);

	SafeDelete(recall);
	SafeDelete(combat);
	SafeDelete(clipInSight);
	SafeDelete(pat);
}

void FieldBehavior::Update()
{
	if (player != nullptr)
	{
		player->Focus(&focusPosition);
	}

	transform->Position(&minePosition);
	reader->Update();
}

void FieldBehavior::InRange()
{
	desc.MovingSystem->GetTransform()->Position(&recall->GetDesc()->Point);

	// 실행 순서 (스택)
	// 4. 순찰
	// 3. 되돌아가기
	// 2. 전투태세
	// 1. 적확인

	reader->PushBack(pat);
	reader->PushBack(recall);
	reader->PushBack(combat);
	reader->PushBack(clipInSight);
}

//void FieldBehavior::OutRange()
//{
//}
