#include "Framework.h"
#include "ClipTimer.h"

ClipTimer::ClipTimer()
{
}

ClipTimer::~ClipTimer()
{
	for (auto d : timerNotifies)
		SafeDelete(d);
}

void ClipTimer::Update()
{
	if (mode == -1) return;

	float deltaTime = Time::Delta() * speed * frameRate;
	runningTime += deltaTime;

	while (currNotify > timerNotifies.size())
	{
		if (timerNotifies[currNotify]->Time > runningTime)
		{
			break;
		}

		timerNotifies[currNotify]->Notify();
		++currNotify;
	}

	if (mode == 0)
		UpdateLoop();
	else if (mode == 1)
		UpdateOnce();
}

void ClipTimer::SetClip(int _clipNum, float _duration, float _frameRate)
{
	clipNum = _clipNum;
	duration = _duration;
	frameRate = _frameRate;
}

void ClipTimer::PlayLoop(float _speed, float _offsetTime)
{
	mode = 0;

	speed = _speed;
	runningTime = _offsetTime;
}

void ClipTimer::PlayOnce(float _speed, float _offsetTime)
{
	mode = 1;

	speed = _speed;
	runningTime = _offsetTime;
}

void ClipTimer::Stop() 
{
	mode = -1; 
}

void ClipTimer::Clear()
{
	clipNum = -1;
	frameRate = 1.0f;
	duration = 0.0f;
	speed = 1.0f;
	runningTime = 0.0f;

	mode = -1;
}

void ClipTimer::AddNotifyTimer(const AnimNotify & value, float time)
{
	vector<TimerNotify*>::iterator test = timerNotifies.begin();

	// 소트 검사
	while (test == timerNotifies.end())
	{
		if ((*test)->Time > time)
		{
			timerNotifies.insert(test, new TimerNotify{ value, time });
			return;
		}
		++test;
	}

	// 가장 늦은 알림
	timerNotifies.push_back(new TimerNotify{value, time});
}

void ClipTimer::UpdateLoop()
{
	float dt = runningTime - duration;
	if (dt > 0.0f)
		runningTime = dt;
}

void ClipTimer::UpdateOnce()
{
	float dt = runningTime - duration;
	if (dt > 0.0f)
	{
		runningTime = duration;
		mode = -1;
	}
}
