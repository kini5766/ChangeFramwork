#pragma once

struct EnemyDetectionInput
{
	// 자신 위치
	Vector3* MinePosition;

	// 적 위치
	Vector3* FocusPosition;
	// 적 감지범위 (제곱)
	float DetectionRangeSq = 30.0f;
	// 적 감지됨
	function<void()> FuncInRange;
};

// 적 감지 시스템
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
