#pragma once

// ModelAnimation -> ���� ��Ŭ���� -> Animator
class Animator
{
public:
	Animator();
	Animator(class ModelAnimation* modelAnim);
	~Animator();

public:
	void Play(UINT clip) { inputNext = (int)clip; }
	void PlayTempBlend(UINT next, float tweenTime);
	void SetCurrBlendEdge(UINT start, UINT end);

public:
	void Update();
	void UpdateBlend();
	void UpdateClip();

public:
	void AddNode(float duration, float speed = 1.0f, float frameRate = 1.0f, function<void(void)> funcPlay = []() {});
	void AddBlendEdge(UINT start, UINT end, float tweenTime, bool bDefault = false);
	void GetAnimDesc(struct BlendDesc* outDesc);

public:
	// get clip

private:
	void NextInput();

private:
	float blendAlpha = 0.0f;
	UINT currClip = 0;
	int inputNext = -1;  // -1�� �Է� ����
	bool bTempEdge = false;


private:
	struct BlendEdge
	{
		UINT Start;
		UINT End;
		// ��ȯ �ð�
		float TweenTime;
	}*currEdge;//, *inputEdge;

private:
	void PlayNextClip(BlendEdge * next, bool bTemp);
	void EndBlend();

private:
	struct ClipNode
	{
		// Ŭ�� ��ȣ
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