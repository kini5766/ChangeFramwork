#pragma once

/*
null üũ + OnAction �� action�� null�� ����
*/
class FutureReturn
{
public:
	FutureReturn();
	~FutureReturn();

public:
	void SetAction(const FutureAction* value = nullptr);
	void Clear() { action = nullptr; }
	bool IsValid() { return action != nullptr; }

	// action�� null�� �ٲ� �� action�� �ִ� �Լ��� ȣ��
	// action�� �ִ� �Լ� ȣ�� �� SetAction�Լ� ȣ�� ���
	void OnAction();

private:
	const FutureAction* action;
};
