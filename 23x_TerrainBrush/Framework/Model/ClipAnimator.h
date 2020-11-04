#pragma once

class ClipAnimator
{
public:
	ClipAnimator(ModelClip** clips, UINT clipCount);
	~ClipAnimator();

public:
	void UpdateNoTweening();
	void UpdateTweening();
	void Render();

public:
	void CreateBuffer(Shader* shader);
	void PlayClip(UINT clip, float speed, float takeTime);
	float GetClipLength(UINT clip);
	float GetClipRunTime();

private:
	struct KeyframeDesc;
	void UpdateClipLoop(KeyframeDesc& desc, ModelClip* clip);
	void NextClip();

private:
	ModelClip** clips;
	UINT clipCount;
	ConstantBuffer* frameBuffer = nullptr;
	ID3DX11EffectConstantBuffer* sFrameBuffer = nullptr;

private:
	struct KeyframeDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.0f;
		float RunningTime = 0.0f;

		float Speed = 1.0f;
		float Padding[2];
	}/*keyframeDesc*/;

	// 동작과 보간
	struct TweenDesc
	{
		float TakeTimeDiv = 1.0f;  // 동작이 섞이는 시간
		float TweenTime = 0.0f;  // 섞이고 있는 중 시간
		float RunningTime = 0.0f;
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;  // -1은 없다 간주
		}
	} tweenDesc;
};