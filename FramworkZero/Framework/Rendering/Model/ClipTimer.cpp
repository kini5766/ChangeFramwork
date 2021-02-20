#include "Framework.h"
#include "ClipTimer.h"

ClipTimer::ClipTimer()
{
}

ClipTimer::~ClipTimer()
{
}

void ClipTimer::Update()
{
	if (mode == -1) return;

	float deltaTime = Time::Delta() * speed * frameRate;
	runningTime += deltaTime;

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

void ClipTimer::Clear()
{
	clipNum = -1;
	frameRate = 1.0f;
	duration = 0.0f;
	speed = 1.0f;
	runningTime = 0.0f;

	mode = -1;
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
