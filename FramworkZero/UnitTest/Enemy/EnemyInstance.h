#pragma once

class EnemyInstance
{
public:
	EnemyInstance(ModelSkinnedInstance* instance, class IFocus* player);
	~EnemyInstance();

public:
	void Update();
	void Render();


private:
	void OnNextAnimation(UINT next);  // 애니메이션 이벤트
	void NextAtteck(UINT& next, const Vector3& dest);
	void NextRun(UINT& next, const Vector3 & dest);
	void NextIdle(UINT& next);

private:
	Transform* transform;

	class PaladinInstance* character;
	class HPSystem* hp;
	class EnemyAttackSystem* attack;
	class IFocus* player;
	UINT currAction = 0;

	float detectionRange = 30.0f;
	float attackRange = 5.0f;
	bool bWariness = false;
	class RotateSystem* ratate;

	float runSpeed = 6.0f;
	float walkSpeed = 3.0f;
	float turnSpeed = 1.0f;

	float idleRunTime = 0.0f;
	UINT currPatrol = 0;
	vector<Vector3> patrolPoints;
};