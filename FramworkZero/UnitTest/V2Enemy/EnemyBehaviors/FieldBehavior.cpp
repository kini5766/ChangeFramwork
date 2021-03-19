#include "stdafx.h"
#include "FieldBehavior.h"

#include "FlowFunction/SimpleBehaviors/SimpleBehaviorHeaders.h"
#include "V2Enemy/EnemyDetectionSystem.h"
#include "Component/PointMoveSystem.h"
#include "Patrolling.h"

FieldBehavior::FieldBehavior(const FieldBehaviorInput & input)
	: desc(input)
	, transform(input.Trans)
	, inRangeDetection(bind(&FieldBehavior::InRangeDetection, this))
{
	EnemyDetectionInput inputED;
	inputED.MinePosition = &minePosition;
	inputED.FocusPosition = &focusPosition;
	inputED.FuncInRange = inRangeDetection;
	desc.DetectionSystem = new EnemyDetectionSystem(inputED);

	pat = new Patrolling(desc);
	clipInSight = new ClipOncePlayer(desc.MakeInSight());

	PointMoverDesc inputPM;
	inputPM.MovingSystem = desc.MoveSystem;
	//inputPM.CallAnim = [=]() { desc.Anim->Play(desc.ClipRun); MovingSystem->SetMoveSpeeder(RunSpeed); };
	recall = new PointMover(inputPM);

	reader = new FlowReader();
	reader->PushBack(pat);
	reader->Call();
}

FieldBehavior::~FieldBehavior()
{
	SafeDelete(reader);

	SafeDelete(recall);
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

void FieldBehavior::InRangeDetection()
{
	desc.MoveSystem->GetTransform()->Position(&recall->GetDesc()->Point);

	// 실행 순서 (스택)
	// 5. 순찰
	// 3. 되돌아가기
	// 2. 전투태세
	// 1. 적확인

	reader->PushBack(pat);
	reader->PushBack(recall);
	reader->PushBack(clipInSight);
}
