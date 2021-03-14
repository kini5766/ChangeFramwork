#pragma once

/*
null üũ + OnAction �� action�� null�� ����
*/
class DelayReturn
{
public:
	DelayReturn();
	~DelayReturn();

public:
	void SetAction(const ReturnAction* value = nullptr);
	void Clear() { action = nullptr; }
	bool IsValid() { return action != nullptr; }

	// action�� null�� �ٲ� �� action�� �ִ� �Լ��� ȣ��
	// action�� �ִ� �Լ� ȣ�� �� SetAction�Լ� ȣ�� ���
	void OnAction();

private:
	const ReturnAction* action;
};
