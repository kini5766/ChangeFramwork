#pragma once

// 매 프레임 쓰레드 생성하는데
// 쓰레드 생성이 느려서 사용안 함

#pragma region TaskDesc

// 스레드 주문서 ThreadGroup에 AddWorking할 때 사용
struct TaskDesc
{
	~TaskDesc() { if (PThis != nullptr) { (*PThis) = nullptr; } }
	TaskDesc** PThis = nullptr;  // 객체 파괴 되면 변수의 값을 nullptr로 바꿔줌 (바꿀 값이 nullptr이면 안 바꿈)

	function<void()> Working;  // 호출될 함수
	bool IsAvailable = true;  // 호출해도 되는지(주문 취소 시 false로 바꾸기)

	float CreatedTime = -2.0f;  // 주문시간 (디버그용)
	string Etc = "None";  // 디버그 시 추가 설명(디버그용)
};

#pragma endregion


#pragma region ThreadToken

// 쓰레드 클래스 (생성시 쓰레드 시작)
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
	float createdTime;  // 생성된 프레임 (디버그용)
};

#pragma endregion


class GameLogic
{
public:
	GameLogic();
	~GameLogic();

public:
	// 스레드 추가하기
	// TaskDesc는 new할당 한 것을 보내야하고 delete는 ThreadGroup에서 한다.
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
	const size_t maxToken = 8;  // 최대 스레드 양

	queue<TaskDesc*> updates;
	queue<TaskDesc*> lateUpdates;
	mutex tasksLocker;
};