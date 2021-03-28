#pragma once

struct BlendData
{
	//UINT Start;  // 소유한 ClipData의 인덱스 번호
	UINT End;
	float TweeningTime = 0.1f;
	bool bDefault = false;
};
struct ClipData
{
	//UINT ClipNum // 인덱스가 클립번호
	float Duration;
	float Speed = 1.0f;
	float FrameRate;
	vector<BlendData> Blends;
};
struct AnimData
{
	vector<ClipData> Clips;
};


class Animator
{
public:
	Animator();
	Animator(const AnimData& data);
	~Animator();

public:
	void Play(UINT clip) { inputNext = (int)clip; }
	void PlayUpdate(UINT clip);  // 현재클립이랑 다를 경우에만 적용
	void PlayTempBlend(UINT next, float tweenTime);  // 즉시재생
	void SetCurrBlendEdge(UINT start, UINT end);  // 즉시재생 (테스트용)
	void SetblendAlpha(float value) { blendAlpha = value; }  // blendAlpha (테스트용)

public:
	void Update();
	void UpdateBlend();
	void UpdateClip();

public:
	void SetSpeed(UINT clip, float speed);
	void AddNode(float duration, float speed = 1.0f, float frameRate = 1.0f);
	void AddBlendEdge(UINT start, UINT end, float tweenTime, bool bDefault = false);
	void GetAnimDesc(struct BlendDesc* outDesc);

	void AddFuncChange(UINT clip, const AnimNotify& value);
	// 타이머에 노티파이 추가 (시간 지정)
	void AddNotifyTimer(UINT clip, const AnimNotify& value, float time);

private:
	void NextInput();

private:
	float blendAlpha = 0.0f;
	UINT currClip = 0;
	int inputNext = -1;  // -1은 입력 없음
	bool bTempEdge = false;


private:
	struct BlendEdge
	{
		UINT Start;
		UINT End;
		// 전환 시간
		float TweenTime;
	}*currEdge = nullptr;//, *inputEdge;

private:
	void PlayNextClip(BlendEdge * next, bool bTemp);
	void EndBlend();

private:
	struct ClipNode
	{
		// 클립 번호
		UINT ClipNum = 0;
		ClipTimer Timer;
		float Speed = 0.0f;

		BlendEdge* DefaultEdge = nullptr;
		vector<BlendEdge*> Edges;
		vector<AnimNotify> OnChanges;

		BlendEdge* EdgeByNextClip(UINT nextClip)
		{
			for (auto f : Edges)
				if (f->End == nextClip)
					return f;
			return nullptr;
		}
	};
	vector<ClipNode*> nodes;
};