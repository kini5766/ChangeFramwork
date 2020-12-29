#pragma once

// ModelAnimation -> 각각 모델클래스 -> Animator
class Animator
{
public:
	Animator();
	Animator(class ModelAnimation* modelAnim);
	~Animator();

public:
	void Play(UINT clip) { inputNext = (int)clip; }
	void PlayTempBlend(UINT next, float tweenTime);
	void Update();

public:
	void AddNode(float duration, float speed = 1.0f, float frameRate = 1.0f, function<void(void)> funcPlay = []() {});
	void AddBlendEdge(UINT start, UINT end, float tweenTime, bool bDefault = false);
	void GetAnimDesc(struct KeyframeDesc* outDesc);

private:
	void UpdateTweening();
	void UpdateClip();
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
		float TweenTime = 0.1f;
	}*currEdge;

private:
	void PlayNextClip(BlendEdge * next, bool bTemp);
	void EndTween();

private:
	struct ClipNode
	{
		// 클립 번호
		UINT ClipNum;
		ClipTimer Time;
		float Speed;
		function<void(void)> Play;

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