#pragma once

class EnemyInstance
{
public:
	EnemyInstance(const struct EnemyInstanceDesc* desc);
	~EnemyInstance();

public:
	void Update();
	void Render();


private:
	void UpdateState();
	void NextAtteck(UINT& next, const Vector3& dest);
	void NextRun(UINT& next, const Vector3 & dest);
	void NextIdle(UINT& next);

	// 이벤트 함수
private:
	void OnNextAnimation(UINT next);  // 애니메이션 이벤트
	void OnDamage();  // hp 이벤트

private:
	// EnemyInstanceDesc
	class IFocus* player;  // 적의 대상 (위치)
	Transform* transform;  // 모델 위치
	Animator* animator;  // 모델 애니메이터
	class AttackAnimation* attack;  // 공격력, 공격 범위
	vector<Vector3> patrolPoints;  // 순찰 경로
	float runSpeed;  // 전투 시 이동 속도
	float walkSpeed;  // 순찰 속도
	float turnSpeed;  // 회전 속도
	float detectionRange;  // 경계 범위
	float attackRange;  // 공격 범위

private:
	class RotateSystem* ratate;  // 회전 보간
	class HPSystem* hp;  // 체력, 공격 받는 범위

	UINT currPatrol = 0;  // 현재 순찰하고 있는 위치
	// 0 : idle, 1 : walk, 2 : run, 3 : taunt, 4 : attack, 5 : react, 6 : fall
	UINT currAction = 0;

	float idleRunTime = 0.0f;  // 전투 종료, 순찰 완료 시 대기한 시간
	float reactTime = 1.0f;  // 경직 상태 길이
	float reactRunTime = 0.0f;  // 남은 경직 시간

	bool bWariness = false;  // 전투 상태
	bool bFall = false;  // 체력 0상태
	bool bLost = false;  // 렌더 안 하는 상태 (릴리즈)
};