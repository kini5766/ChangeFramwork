#include "stdafx.h"
#include "FieldEnemy.h"

#include "EnemyDetectionSystem.h"
#include "EnemyBehaviors/Patrolling.h"

FieldEnemy::FieldEnemy(const FieldEnemyInput & input)
	: desc(input)
	, inRangeDetection(bind(&FieldEnemy::InRangeDetection, this))
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

FieldEnemy::~FieldEnemy()
{
	SafeDelete(reader);
	SafeDelete(pat);
}

void FieldEnemy::Update()
{
	reader->Update();
}

void FieldEnemy::InRangeDetection()
{
	// 실행 순서 (스택)
	// 4. 순찰
	// 3. 되돌아가기
	// 2. 전투태세
	// 1. 적확인


}
