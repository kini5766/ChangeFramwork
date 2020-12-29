#pragma once

struct KeyframeDesc
{
	int Clip = -1;
	float Time = 0.0f;
	float Padding[2];
	//float RunningTime = 0.0f;
	//float Speed = 1.0f;
};

struct BlendDesc
{
	float Alpha = 0;
	float Padding[3];

	KeyframeDesc Clip[2];
};
