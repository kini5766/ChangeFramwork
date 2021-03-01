#pragma once

struct EnemyDetectionInput
{
	// �ڽ� ��ġ
	Vector3* MinePosition;

	// �� ��ġ
	Vector3* FocusPosition;
	// �� �������� (����)
	float DetectionRangeSq = 30.0f;
	// �� ������
	function<void()> FuncInRange;
};

// �� ���� �ý���
class EnemyDetectionSystem
{
public:
	EnemyDetectionSystem(const EnemyDetectionInput& input);
	~EnemyDetectionSystem();

public:
	bool IsInRange();
	void OnInRange();

private:
	EnemyDetectionInput desc;
};
