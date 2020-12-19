#include "Framework.h"
#include "GameLogic.h"


#pragma region ThreadToken

ThreadToken::ThreadToken(TaskDesc* desc)
	: desc(desc)
{
	createdTime = Time::Get()->Running();

	// ���� �ֹ��� ��ȿ�ϸ�
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
	// static�� ��Ƽ �����忡�� ȣ�� �� ������ �߻��ϴ� ������ mutex�� ���
	instance->tasksLocker.lock();
	instance->updates.push(desc);
	instance->tasksLocker.unlock();
}

void GameLogic::AddLateUpdate(TaskDesc * desc)
{
	// static�� ��Ƽ �����忡�� ȣ�� �� ������ �߻��ϴ� ������ mutex�� ���
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
		// �۾� ���� ������ �Ҵ� ����
		list<ThreadToken*>::iterator iter = tokens.begin();
		while (iter != tokens.end())
		{
			if ((*iter)->IsPlaying())
			{
				++iter;
				continue;
			}

			// �۾��� ���� ������ ã��
			SafeDelete(*iter);
			iter = tokens.erase(iter);
		}

		// ���� ������ ������ ���� �ֹ��� ����
		while (tokens.size() < maxToken)
		{
			if (tasks.empty())
				break;  // �ֹ� ����

			// ��ȿ�� �ֹ��� ���� �ƴϸ� ���
			TaskDesc * desc = tasks.front();
			if (desc->IsAvailable)
			{
				// ���� ���� ���� �۾� ������ ����
				tokens.push_back(new ThreadToken(desc));
			}
			else
			{
				// ���� ���� �ֹ��� ���� �� ���� �ֹ� Ȯ��
				delete(desc);
				tasks.front() = nullptr;
			}

			// ���� �ֹ�
			tasks.pop();
		}

		// ���� �� ������ ����
		if (tokens.size() == 0) { break; }
	}

}

#pragma endregion
