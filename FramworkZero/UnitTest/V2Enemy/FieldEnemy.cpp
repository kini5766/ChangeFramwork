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
	// ���� ���� (����)
	// 4. ����
	// 3. �ǵ��ư���
	// 2. �����¼�
	// 1. ��Ȯ��


}
