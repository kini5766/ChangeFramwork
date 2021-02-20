#pragma once

class ClipTimer
{
public:
	ClipTimer();
	~ClipTimer();

public:
	void Update();

public:
	void SetClip(int _clipNum, float _duration, float _frameRate = 1.0f);
	void PlayLoop(float _speed = 1.0f, float _offsetTime = 0.0f);
	void PlayOnce(float _speed = 1.0f, float _offsetTime = 0.0f);
	void SetRunningTime(float value) { runningTime = value; }
	void Stop() { mode = -1; }
	void Clear();

public:
	int ClipNum() { return clipNum; }
	float FrameRate() { return frameRate; }
	float Duration() { return duration; }
	float Speed() { return speed; }
	float RunningTime() { return runningTime; }
	bool IsPlaying() { return mode != -1; }

private:
	void UpdateLoop();
	void UpdateOnce();

private:
	// 현재 클립 인덱스 -1은 재생 중이 아님
	int clipNum = -1;
	float frameRate = 1.0f;
	float duration = 0.0f;
	float speed = 1.0f;
	float runningTime = 0.0f;

	UINT mode = -1; //  -1 : Stop, 0 : Loop, 1 : Once
};