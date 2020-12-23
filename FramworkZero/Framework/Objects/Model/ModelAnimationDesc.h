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
	UINT Mode = 0;  // bool 1바이트 애매함
	float Alpha = 0;
	float Padding[2];

	KeyframeDesc Clip[2];
};
