#include "Framework.h"
#include "ClipAnimator.h"

ClipAnimator::ClipAnimator(ModelClip ** clips, UINT clipCount)
	: clips(clips), clipCount(clipCount)
{
}

ClipAnimator::~ClipAnimator()
{
	SafeDelete(frameBuffer);
}

void ClipAnimator::UpdateNoTweening()
{
	TweenDesc& desc = tweenDesc;
	ModelClip* clip = clips[desc.Curr.Clip];

	// 현재 애니메이션
	UpdateClipLoop(desc.Curr, clip);

	// 다음 애니메이션
	if (desc.Next.Clip > -1)
	{
		ModelClip* nextClip = clips[desc.Curr.Clip];
		NextClip();
		UpdateClipLoop(desc.Next, nextClip);
	}
}

void ClipAnimator::UpdateTweening()
{
	//KeyframeDesc& desc = keyframeDesc;
	TweenDesc& desc = tweenDesc;
	ModelClip* clip = clips[desc.Curr.Clip];

	// 현재 애니메이션
	UpdateClipLoop(desc.Curr, clip);

	// 다음 애니메이션
	if (desc.Next.Clip > -1)
	{
		ModelClip* nextClip = clips[desc.Next.Clip];

		//-> 동작 전환 간 선형보간

		desc.RunningTime += Time::Delta();
		desc.TweenTime = desc.RunningTime * desc.TakeTimeDiv;

		if (desc.TweenTime >= 1.0f)
			NextClip();
		else 
			UpdateClipLoop(desc.Next, nextClip);
	}
}

void ClipAnimator::Render()
{
	if (frameBuffer == nullptr)
		return;

	frameBuffer->Render();
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
}

void ClipAnimator::CreateBuffer(Shader * shader)
{
	SafeDelete(frameBuffer);
	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc));
	sFrameBuffer = shader->AsConstantBuffer(ShaderEffctConstantName::CB_AnimationFrame);
}


void ClipAnimator::PlayClip(UINT clip, float speed, float takeTime)
{
	tweenDesc.TakeTimeDiv = 1.0f / takeTime;
	tweenDesc.Next.Clip = clip;
	tweenDesc.Next.Speed = speed;
}

float ClipAnimator::GetClipLength(UINT clip)
{
	return static_cast<float>(clips[clip]->FrameCount());
}

float ClipAnimator::GetClipRunTime()
{
	float result = 0.0f;
	if (tweenDesc.Next.Clip == -1)
		result = tweenDesc.Curr.RunningTime;
	else
		result = tweenDesc.Next.RunningTime;
	return result;
}

void ClipAnimator::UpdateClipLoop(KeyframeDesc& desc, ModelClip* clip)
{
	float time = clip->FrameRate() * desc.Speed * Time::Delta();

	int nextSize = static_cast<int>(desc.Time);
	desc.Time -= static_cast<float>(nextSize);
	if (desc.Time < 0.0f)
	{
		// 역 재생
		desc.Time += 1.0f;
		nextSize--;
	}

	if (nextSize != 0)
	{
		// desc.CurrFrame, desc.NextFrame이 unsinged int라서 음수 값 없애기
		nextSize = nextSize % static_cast<int>(clip->FrameCount());
		if (nextSize < 0)
			nextSize += clip->FrameCount();

		desc.CurrFrame = (desc.CurrFrame + nextSize) % clip->FrameCount();  // Loop
		desc.NextFrame = (desc.CurrFrame + 1) % clip->FrameCount();  // Loop
	}

	desc.Time += time;
	desc.RunningTime += time;


	//// 원본
	//desc.RunningTime += Time::Delta();
	//float time = 1.0f / (clip->FrameRate() * desc.Speed);
	////float time = 1.0f / clip->FrameRate() / desc.Speed;

	//if (desc.Time >= 1.0f)
	//{
	//	desc.RunningTime -= desc.Time * time;
	//	//desc.RunningTime = 0.0f;
	//	desc.CurrFrame = (desc.CurrFrame + 1) % clip->FrameCount();  // Loop
	//	desc.NextFrame = (desc.CurrFrame + 1) % clip->FrameCount();  // Loop
	//}
	//desc.Time = desc.RunningTime / time;

}

void ClipAnimator::NextClip()
{
	TweenDesc& desc = tweenDesc;
	desc.Curr = desc.Next;

	desc.Next.Clip = -1;
	desc.Next.CurrFrame = 0;
	desc.Next.NextFrame = 0;
	desc.Next.Time = 0;
	desc.Next.RunningTime = 0.0f;
	desc.RunningTime = 0.0f;
	desc.TweenTime = 0.0f;
}
