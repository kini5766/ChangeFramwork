#pragma once

struct EnemyDesc
{
	Transform* Transform;  // �� ��ġ
	Animator* Animator;  // �� �ִϸ�����
	vector<Vector3> PatrolPoints;  // ���� ���
	float RunSpeed = 6.0f;  // ���� �� �̵� �ӵ�
	float WalkSpeed = 3.0f;  // ���� �ӵ�
	float TurnSpeed = 1.0f;  // ȸ�� �ӵ�
	float DetectionRange = 30.0f;  // ��� ����
	float AttackRange = 5.0f;  // ���� ����

	class AttackAnimation* Attack = nullptr;
};

class EnemyInstance
{
public:
	EnemyInstance(const EnemyDesc& desc, class IFocus* player);
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
	// enemy desc
	Transform* transform;  // �� ��ġ
	Animator* animator;  // �� �ִϸ�����
	vector<Vector3> patrolPoints;  // ���� ���
	float runSpeed;  // ���� �� �̵� �ӵ�
	float walkSpeed;  // ���� �ӵ�
	float turnSpeed;  // ȸ�� �ӵ�
	float detectionRange;  // ��� ����
	float attackRange;  // ���� ����

private:
	class RotateSystem* ratate;  // ȸ�� ����

	class HPSystem* hp;  // ü��, ���� �޴� ����
	class AttackAnimation* attack;  // ���ݷ�, ���� ����
	class IFocus* player;  // ���� ��� (��ġ)
	// 0 : idle, 1 : walk, 2 : run, 3 : taunt, 4 : attack, 5 : react, 6 : fall
	UINT currAction = 0;

	bool bWariness = false;  // ���� ����

	float idleRunTime = 0.0f;  // ���� ����, ���� �Ϸ� �� ����� �ð�
	UINT currPatrol = 0;  // ���� �����ϰ� �ִ� ��ġ

	float reactTime = 1.0f;  // ���� ���� ����
	float reactRunTime = 0.0f;  // ���� ���� �ð�
	bool bFall = false;  // ü�� 0����
	bool bLost = false;  // ���� �� �ϴ� ���� (������)
};