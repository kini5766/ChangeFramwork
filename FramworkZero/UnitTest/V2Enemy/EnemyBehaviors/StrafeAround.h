#pragma once

#include "EnemyBehaviorDesc.h"
#include "FlowFunction/FlowFunction.h"


// �߰� ���� �� : �ֺ� �ɵ���
// �߰� ���� �� : ���󰡱�
class StrafeAround : public IFlowTesk
{
public:
	StrafeAround(const StrafeAroundDesc& input);
	~StrafeAround();


private:
	void Call(const FutureAction * action) override;
	void Update() override;
	void Hold() override;

private:
	// �ൿ ĳ�̽� ������Ʈ
	int UpdateCase();
	FuncCase funcCase;

	// ����
	void EndCase();
	FutureAction funcEndCase;

	// �̵��� ��ġ ���ϱ�
	void UpdatePositioning();
	FutureAction funcPositioning;

private:
	StrafeAroundDesc desc;
	FutureReturn result;
	FlowSwitching* switching;

	FlowAction* playIdle;
	FlowAction* playRun;

	class Waiter* waiter;
	class Follower* follower;
	class PointMover* moveto;

	FlowRoutine* wait;
	FlowRoutine* move;
	FlowAction* positioning;

	FlowRoutine* followRoutine;
	FlowRoutine* prowlRoutine;

private:
	bool curr = false;

	// �� ��ġ�� ȣ��
	function<void()> funcOutRange;
public:
	void SetFuncOutRange(const function<void()>& value) { funcOutRange = value; }
};
