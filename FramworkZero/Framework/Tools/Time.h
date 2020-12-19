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
	bool isTimerStopped;// Ÿ�̸� ����
	float timeElapsed;// ���� ���������κ��� ����ð�

	INT64 currentTime;// ���� �ð�
	INT64 lastTime;// �����ð�

	float runningTime;// ���� �ð�

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
	INT64 lastFPSUpdate;// ������ FPS ������Ʈ �ð�
	INT64 fpsUpdateInterval;// fps ������Ʈ ����
	UINT frameCount;// fps üũ ������ ��
	float framePerSecond;// FPS

	INT64 ticksPerSecond;// �ʴ� ƽī��Ʈ
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