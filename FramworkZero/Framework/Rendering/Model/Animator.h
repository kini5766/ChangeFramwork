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
	void PlayTempBlend(UINT next, float tweenTime);
	void SetCurrBlendEdge(UINT start, UINT end);
	void SetblendAlpha(float value) { blendAlpha = value; }

public:
	void Update();
	void UpdateBlend();
	void UpdateClip();

public:
	void SetSpeed(UINT clip, float speed);
	void AddNode(float duration, float speed = 1.0f, float frameRate = 1.0f);
	void AddBlendEdge(UINT start, UINT end, float tweenTime, bool bDefault = false);
	void SetFuncNext(function<void(UINT)> value) { funcNext = value; }
	void GetAnimDesc(struct BlendDesc* outDesc);

private:
	void NextInput();

private:
	float blendAlpha = 0.0f;
	UINT currClip = 0;
	int inputNext = -1;  // -1은 입력 없음
	bool bTempEdge = false;
	function<void(UINT clip)> funcNext = [](UINT) {};


private:
	struct BlendEdge
	{
		UINT Start;
		UINT End;
		// 전환 시간
		float TweenTime;
	}*currEdge;//, *inputEdge;

private:
	void PlayNextClip(BlendEdge * next, bool bTemp);
	void EndBlend();

private:
	struct ClipNode
	{
		// 클립 번호
		UINT ClipNum;
		ClipTimer Time;
		float Speed;

		BlendEdge* DefaultEdge;
		vector<BlendEdge*> Edges;

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