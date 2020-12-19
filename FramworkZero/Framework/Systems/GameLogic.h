#pragma once

// �� ������ ������ �����ϴµ�
// ������ ������ ������ ���� ��

#pragma region TaskDesc

// ������ �ֹ��� ThreadGroup�� AddWorking�� �� ���
struct TaskDesc
{
	~TaskDesc() { if (PThis != nullptr) { (*PThis) = nullptr; } }
	TaskDesc** PThis = nullptr;  // ��ü �ı� �Ǹ� ������ ���� nullptr�� �ٲ��� (�ٲ� ���� nullptr�̸� �� �ٲ�)

	function<void()> Working;  // ȣ��� �Լ�
	bool IsAvailable = true;  // ȣ���ص� �Ǵ���(�ֹ� ��� �� false�� �ٲٱ�)

	float CreatedTime = -2.0f;  // �ֹ��ð� (����׿�)
	string Etc = "None";  // ����� �� �߰� ����(����׿�)
};

#pragma endregion


#pragma region ThreadToken

// ������ Ŭ���� (������ ������ ����)
class ThreadToken
{
public:
	ThreadToken(TaskDesc* desc);
	~ThreadToken();

public:
	bool IsPlaying() { return bPlaying; }

private:
	void Work();

private:
	TaskDesc* desc;
	bool bPlaying = true;
	float createdTime;  // ������ ������ (����׿�)
};

#pragma endregion


class GameLogic
{
public:
	GameLogic();
	~GameLogic();

public:
	// ������ �߰��ϱ�
	// TaskDesc�� new�Ҵ� �� ���� �������ϰ� delete�� ThreadGroup���� �Ѵ�.
	static void AddUpdate(TaskDesc* desc);
	static void AddLateUpdate(TaskDesc* desc);

	static void Set(GameLogic* value);

private:
	static GameLogic* instance;

public:
	void Update();
	void LateUpdate();

private:
	void WorkTasks(queue<TaskDesc*>& target);

private:
	const size_t maxToken = 8;  // �ִ� ������ ��

	queue<TaskDesc*> updates;
	queue<TaskDesc*> lateUpdates;
	mutex tasksLocker;
};