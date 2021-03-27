#pragma once

#include "V2EnemyDesc.h"

/*
순찰>[적인식]>적확인>[종료시]>전투
전투>[스킬사용가능시]>스킬>[종료시]>전투
전투>[적놓침]>되돌아가기>[종료시]>순찰

공격받음, 쓰러짐은 중간에 끼워넣음
*/
class FieldEnemy
{
public:
	FieldEnemy(const FieldEnemyInput& desc);
	~FieldEnemy();


public:
	void Update();
	void PostRender();

private:
	void OnDamage();
	void OnLost();
	function<void()> onLost;
	void InRange();

private:
	V2EnemyStatus oriStatus;  // 원본 능력치
	V2EnemyStatus status;  // 적용 능력치
	class IFocus* player;  // 적의 대상 (위치)
	Transform* transform;  // 모델 위치
	Animator* animator;  // 모델 애니메이터
	vector<Vector3> patrolPoints;  // 순찰 경로

	class HPSystem* hp;  // 체력, 공격 받는 범위
	//FieldEnemyClipNumGroup clipGroup;

	bool bDead = false;
	bool bLost = false;  // 죽어서 안 보이는 상태


	// -- Field Behavior Service -- //
private:
	class PerceptionSystem* perceptor;
	class PointMoveSystem* movingSystem;

	// -- Field Behavior -- //
private:
	class FlowReader* reader;

	class Patrolling* pat;

	class FlowRoutine* recall;
	class FlowAction* playRun;
	class PointMover* moveto;

	class CombatPosture* combat;

	class ClipOncePlayer* clipInSight;
	class ClipOncePlayer* clipHitted;
	class ClipOncePlayer* clipDead;
};


// 남은 것
// 적 공격
// 적 다양화