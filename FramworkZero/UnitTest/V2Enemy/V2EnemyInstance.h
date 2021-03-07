#pragma once

#include "V2EnemyDesc.h"

class V2EnemyInstance
{
public:
	V2EnemyInstance(const V2EnemyInstanceDesc& desc);
	~V2EnemyInstance();


public:
	void Update();
	void PostRender();

private:
	void OnDamage();


private:
	V2EnemyStatus oriStatus;  // 원본 능력치
	class IFocus* player;  // 적의 대상 (위치)
	Transform* transform;  // 모델 위치
	Animator* animator;  // 모델 애니메이터
	vector<Vector3> patrolPoints;  // 순찰 경로

	V2EnemyClipNumGroup clipGroup;

private:
	class HPSystem* hp;  // 체력, 공격 받는 범위
	class FieldBehavior* behavior;
};