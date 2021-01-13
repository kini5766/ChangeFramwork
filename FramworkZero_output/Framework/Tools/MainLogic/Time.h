#pragma once

class Time
{
private:
	static Time* global;

public:
	static Time* Get();

	static bool Stopped() { return global->LocalStopped(); }
	static float Delta() { return global->LocalDelta(); }

	static void SetGlobal(Time* value);

public:
	Time();
	~Time();

public:
	void Update();

	void Start();
	void Stop();

	float Running() const { return runningTime; }

	bool LocalStopped() { return isTimerStopped; }
	float LocalDelta() { return isTimerStopped ? 0.0f : timeElapsed; }

private:
	bool isTimerStopped;// 타이머 중지
	float timeElapsed;// 이전 프레임으로부터 경과시간

	INT64 currentTime;// 현재 시간
	INT64 lastTime;// 이전시간

	float runningTime;// 진행 시간

	// fps
public:
	float FPS();

private:
	class TimeFPS* fps;
	INT64 ticksPerSecond;
};

///////////////////////////////////////////////////////////////////////////////

class TimeFPS
{
public:
	TimeFPS(INT64 ticksPerSecond);
	~TimeFPS();

public:
	void Update();
	float FPS() { return framePerSecond; }

private:
	INT64 lastFPSUpdate;// 마지막 FPS 업데이트 시간
	INT64 fpsUpdateInterval;// fps 업데이트 간격
	UINT frameCount;// fps 체크 프레임 수
	float framePerSecond;// FPS

	INT64 ticksPerSecond;// 초당 틱카운트
};

///////////////////////////////////////////////////////////////////////////////

class Timer
{
public:
	Timer();
	~Timer();

	void Start(function<void()> func, int milliSec, UINT repeat = 0);
	void Stop();

private:
	mutex m;

	bool bComplete;
	UINT count;
};

///////////////////////////////////////////////////////////////////////////////

class Performance
{
public:
	Performance();

	void Start();
	float End();
	float Get();

private:
	__int64 tick;
	__int64 start, end;
};