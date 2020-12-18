#include "Framework.h"
#include "GameLogic.h"


#pragma region ThreadToken

ThreadToken::ThreadToken(TaskDesc* desc)
	: desc(desc)
{
	createdTime = Time::Get()->Running();

	// 아직 주문이 유효하면
	if (desc->IsAvailable)
		thread(&ThreadToken::Work, this).detach();
	else bPlaying = false;
}

ThreadToken::~ThreadToken()
{
	delete(desc);
}

void ThreadToken::Work()
{
	desc->Working();
	bPlaying = false;
}

#pragma endregion


#pragma region GameLogic

// static
GameLogic* GameLogic::instance = nullptr;

GameLogic::GameLogic()
{
}

GameLogic::~GameLogic()
{
}

void GameLogic::AddUpdate(TaskDesc* desc)
{
	// static은 멀티 쓰레드에서 호출 시 간섭이 발생하는 오류로 mutex를 사용
	instance->tasksLocker.lock();
	instance->updates.push(desc);
	instance->tasksLocker.unlock();
}

void GameLogic::AddLateUpdate(TaskDesc * desc)
{
	// static은 멀티 쓰레드에서 호출 시 간섭이 발생하는 오류로 mutex를 사용
	instance->tasksLocker.lock();
	instance->lateUpdates.push(desc);
	instance->tasksLocker.unlock();
}

void GameLogic::Set(GameLogic * value)
{
	instance = value;
}

void GameLogic::Update()
{
	WorkTasks(updates);
}

void GameLogic::LateUpdate()
{
	WorkTasks(lateUpdates);
}

void GameLogic::WorkTasks(queue<TaskDesc*>& target)
{
	queue<TaskDesc*> tasks;
	while (target.empty() == false)
	{
		tasks.push(target.front());
		target.pop();
	}

	list<ThreadToken*> tokens;
	while (true)
	{
		// 작업 끝난 스레드 할당 해제
		list<ThreadToken*>::iterator iter = tokens.begin();
		while (iter != tokens.end())
		{
			if ((*iter)->IsPlaying())
			{
				++iter;
				continue;
			}

			// 작업이 끝난 스레드 찾음
			SafeDelete(*iter);
			iter = tokens.erase(iter);
		}

		// 접수 가능한 수까지 남은 주문서 접수
		while (tokens.size() < maxToken)
		{
			if (tasks.empty())
				break;  // 주문 없음

			// 유효한 주문은 접수 아니면 폐기
			TaskDesc * desc = tasks.front();
			if (desc->IsAvailable)
			{
				// 접수 성공 다음 작업 스레드 시작
				tokens.push_back(new ThreadToken(desc));
			}
			else
			{
				// 접수 실패 주문서 삭제 후 다음 주문 확인
				delete(desc);
				tasks.front() = nullptr;
			}

			// 다음 주문
			tasks.pop();
		}

		// 남은 일 없으면 종료
		if (tokens.size() == 0) { break; }
	}

}

#pragma endregion
