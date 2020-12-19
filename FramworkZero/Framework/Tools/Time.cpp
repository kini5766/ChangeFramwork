#include "framework.h"
#include "Time.h"

Time* Time::global = nullptr;

Time* Time::Get()
{
	return global;
}
void Time::SetGlobal(Time * value)
{
	global = value;
}

Time::Time()
	: currentTime(0), lastTime(0), runningTime(0)
	, isTimerStopped(true), timeElapsed(0.0f)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond);
	//ticksPerSecond = ticksPerSecond + (QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond) * 0);
	fps = new TimeFPS(ticksPerSecond);
}

Time::~Time()
{
	SafeDelete(fps);
}

void Time::Update()
{
	if (isTimerStopped) return;

	// 현재시간을 가져와 시간 간격 및 진행 시간을 계산한다.
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);  // 메인보드 클럭한 수(ms)
	timeElapsed = (float)(currentTime - lastTime) / (float)ticksPerSecond;
	runningTime += timeElapsed;

	lastTime = currentTime;

	fps->Update();
}

void Time::Start()
{
	if (!isTimerStopped) return;

	QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	isTimerStopped = false;
}

void Time::Stop()
{
	if (isTimerStopped) return;

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&stopTime);
	runningTime += (float)(stopTime - lastTime) / (float)ticksPerSecond;
	isTimerStopped = true;
}

float Time::FPS() { return fps->FPS(); }

///////////////////////////////////////////////////////////////////////////////

TimeFPS::TimeFPS(INT64 ticksPerSecond)
	: lastFPSUpdate(0), fpsUpdateInterval(0), frameCount(0)
	, ticksPerSecond(ticksPerSecond)
{
	fpsUpdateInterval = ticksPerSecond >> 1;
}

TimeFPS::~TimeFPS()
{
}

void TimeFPS::Update()
{
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);

	frameCount++;
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempDest = ((float)currentTime - (float)lastFPSUpdate);
		framePerSecond = (float)frameCount * (float)ticksPerSecond / tempDest;

		lastFPSUpdate = currentTime;
		frameCount = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////

Timer::Timer()
{
	bComplete = false;

	count = 0;
}

Timer::~Timer()
{

}

void Timer::Start(function<void()> func, int milliSec, UINT repeat)
{
	assert(bComplete == false);

	bComplete = false;
	thread t([=]()
	{
		while (true)
		{
			if (repeat > 0 && count == repeat)
				break;

			if (bComplete == true)
				break;

			count++;
			Sleep(milliSec);

			if (bComplete == true)
				break;

			func();
		}

		Stop();
	});
	t.detach();
}

void Timer::Stop()
{
	count = 0;

	bComplete = true;
}

///////////////////////////////////////////////////////////////////////////////

Performance::Performance()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&tick);
}

void Performance::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&start);
}

float Performance::End()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&end);

	return (float)((double)(end - start) / tick * 1000.0f);
}

float Performance::Get()
{
	return (float)((double)(end - start) / tick * 1000.0f);
}
