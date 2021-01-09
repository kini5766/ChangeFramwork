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

	// �̺�Ʈ �Լ�
private:
	void OnNextAnimation(UINT next);  // �ִϸ��̼� �̺�Ʈ
	void OnDamage();  // hp �̺�Ʈ

private:
	// EnemyInstanceDesc
	class IFocus* player;  // ���� ��� (��ġ)
	Transform* transform;  // �� ��ġ
	Animator* animator;  // �� �ִϸ�����
	class AttackAnimation* attack;  // ���ݷ�, ���� ����
	vector<Vector3> patrolPoints;  // ���� ���
	float runSpeed;  // ���� �� �̵� �ӵ�
	float walkSpeed;  // ���� �ӵ�
	float turnSpeed;  // ȸ�� �ӵ�
	float detectionRange;  // ��� ����
	float attackRange;  // ���� ����

private:
	class RotateSystem* ratate;  // ȸ�� ����
	class HPSystem* hp;  // ü��, ���� �޴� ����

	UINT currPatrol = 0;  // ���� �����ϰ� �ִ� ��ġ
	// 0 : idle, 1 : walk, 2 : run, 3 : taunt, 4 : attack, 5 : react, 6 : fall
	UINT currAction = 0;

	float idleRunTime = 0.0f;  // ���� ����, ���� �Ϸ� �� ����� �ð�
	float reactTime = 1.0f;  // ���� ���� ����
	float reactRunTime = 0.0f;  // ���� ���� �ð�

	bool bWariness = false;  // ���� ����
	bool bFall = false;  // ü�� 0����
	bool bLost = false;  // ���� �� �ϴ� ���� (������)
};