#pragma once

struct PerceptionInput
{
	// �ڽ� ��ġ
	function<Vector3()> FuncGetMine;

	// �� ��ġ
	function<Vector3()> FuncGetFocus;

	// �� ���� ���� (����)
	float SightRangeSq = 0.0f;
	// �� ��ħ ���� (����)
	float LoseRangeSq = 0.0f;
};

// �� ���� �ý���
class PerceptionSystem
{
public:
	PerceptionSystem(const PerceptionInput& input);
	~PerceptionSystem();

public:
	void Update();

	bool IsPerceived();
	float GetDistanceSq() { return distanceSq; }
	Vector3 GetFocus() { return desc.FuncGetFocus(); }
	Vector3 GetDest() { return dest; }

private:
	PerceptionInput desc;
	bool isPerceived = false;
	float distanceSq = 0.0f;
	Vector3 dest;
};
