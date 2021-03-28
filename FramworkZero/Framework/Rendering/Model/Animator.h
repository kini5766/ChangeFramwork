#pragma once

struct BlendData
{
	//UINT Start;  // ������ ClipData�� �ε��� ��ȣ
	UINT End;
	float TweeningTime = 0.1f;
	bool bDefault = false;
};
struct ClipData
{
	//UINT ClipNum // �ε����� Ŭ����ȣ
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
	void PlayUpdate(UINT clip);  // ����Ŭ���̶� �ٸ� ��쿡�� ����
	void PlayTempBlend(UINT next, float tweenTime);  // ������
	void SetCurrBlendEdge(UINT start, UINT end);  // ������ (�׽�Ʈ��)
	void SetblendAlpha(float value) { blendAlpha = value; }  // blendAlpha (�׽�Ʈ��)

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
	// Ÿ�̸ӿ� ��Ƽ���� �߰� (�ð� ����)
	void AddNotifyTimer(UINT clip, const AnimNotify& value, float time);

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
	}*currEdge = nullptr;//, *inputEdge;

private:
	void PlayNextClip(BlendEdge * next, bool bTemp);
	void EndBlend();

private:
	struct ClipNode
	{
		// Ŭ�� ��ȣ
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