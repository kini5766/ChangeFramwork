#include "stdafx.h"
#include "FieldBehavior.h"

#include "V2Enemy/EnemyDetectionSystem.h"
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

	reader = new DelayReader();
	reader->PushBack(pat);
	reader->Call();
}

FieldBehavior::~FieldBehavior()
{
	SafeDelete(reader);
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
	// ���� ���� (����)
	// 4. ����
	// 3. �ǵ��ư���
	// 2. �����¼�
	// 1. ��Ȯ��


}
