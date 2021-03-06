#include "Framework.h"
#include "Animator.h"

Animator::Animator()
{
}

Animator::Animator(const AnimData & data)
{
	for (const ClipData& clip : data.Clips)
	{
		AddNode(clip.Duration, clip.Speed, clip.FrameRate);
	}

	UINT size = data.Clips.size();
	for (UINT i = 0; i < size; i++)
	{
		for (const BlendData& blend : data.Clips[i].Blends)
		{
			AddBlendEdge(i, blend.End, blend.TweeningTime, blend.bDefault);
		}
	}
}

Animator::~Animator()
{
	for (auto node : nodes)
	{
		for (auto d : node->Edges)
			SafeDelete(d);
		SafeDelete(node);
	}

	if (bTempEdge)
		SafeDelete(currEdge);

	//SafeDelete(inputEdge);
}

void Animator::PlayTempBlend(UINT next, float tweenTime)
{
	//SafeDelete(inputEdge);

	BlendEdge* edge = new BlendEdge();
	edge->Start = currClip;
	edge->End = next;
	edge->TweenTime = tweenTime;
	//inputEdge = edge;
	PlayNextClip(edge, true);
}

void Animator::SetCurrBlendEdge(UINT start, UINT end)
{
	BlendEdge* edge = new BlendEdge();
	edge->Start = start;
	edge->End = end;
	edge->TweenTime = 1.0f;
	PlayNextClip(edge, true);
}


void Animator::Update()
{
	// Ʈ����
	UpdateBlend();

	// Ŭ�� ������Ʈ
	UpdateClip();
}

void Animator::UpdateBlend()
{
	// Ʈ���� �� �� ��
	if (currEdge != nullptr)
	{
		blendAlpha += Time::Delta() / currEdge->TweenTime;

		// Ʈ���� ��
		if (blendAlpha >= 1.0f)
			EndBlend();
	}

	// Ʈ���� ���� �ƴ� ��
	else NextInput();

}

void Animator::UpdateClip()
{
	// Ŭ�� ������Ʈ
	ClipNode* curr = nodes[currClip];
	curr->Timer.Update();

	if (currEdge != nullptr)
	{
		// ���� Ŭ�� ������Ʈ
		ClipNode* next = nodes[currEdge->End];
		next->Timer.Update();
	}
	else
	{
		BlendEdge* edge = curr->DefaultEdge;
		float startTime = curr->Timer.Duration();
		startTime -= curr->Timer.FrameRate() * curr->Timer.Speed() * edge->TweenTime;

		// Ŭ�� ����ð� ������ ���� �Է��� ���� ���
		if (curr->Timer.RunningTime() >= startTime)
			PlayNextClip(edge, false);
	}
}

void Animator::SetSpeed(UINT clip, float speed)
{
	nodes[clip]->Speed = speed;
}

void Animator::AddNode(float duration, float speed, float frameRate)
{
	ClipNode* node = new ClipNode();
	node->ClipNum = nodes.size();
	node->Timer.SetClip(node->ClipNum, duration, frameRate);
	node->Speed = speed;
	nodes.push_back(node);

	BlendEdge* edge = new BlendEdge();
	edge->Start = node->ClipNum;
	edge->End = node->ClipNum;
	edge->TweenTime = 0.0f;
	node->Edges.push_back(edge);
	node->DefaultEdge = edge;

	// ó�� ������ ��� ��ȣ (0)
	if (node->ClipNum == currClip)
	{
		ClipNode* curr = nodes[currClip];
		curr->Timer.PlayOnce(curr->Speed);
	}

}

void Animator::AddBlendEdge(UINT start, UINT end, float tweenTime, bool bDefault)
{
	BlendEdge* edge = nodes[currClip]->EdgeByNextClip(inputNext);
	if (edge == nullptr)
	{
		edge = new BlendEdge();
		nodes[start]->Edges.push_back(edge);
	}
	edge->Start = start;
	edge->End = end;
	edge->TweenTime = tweenTime;

	if (bDefault)
		nodes[start]->DefaultEdge = edge;
}

void Animator::GetAnimDesc(BlendDesc * outDesc)
{
	outDesc->Clip[0].Clip = currClip;
	outDesc->Clip[0].Time = nodes[currClip]->Timer.RunningTime();

	if (currEdge != nullptr)
	{
		outDesc->Clip[1].Clip = currEdge->End;
		outDesc->Clip[1].Time = nodes[currEdge->End]->Timer.RunningTime();
		outDesc->Alpha = blendAlpha;
	}
	else
	{
		outDesc->Alpha = 0.0f;
	}
}


void Animator::AddFuncChange(UINT clip, const AnimNotify & value)
{
	nodes[clip]->OnChanges.push_back(value);
}
void Animator::AddNotifyTimer(UINT clip, const AnimNotify & value, float time)
{
	nodes[clip]->Timer.AddNotifyTimer(value, time);
}


void Animator::NextInput()
{
	//if (inputEdge != nullptr)
	//{
	//	PlayNextClip(inputEdge, true);
	//	inputEdge = nullptr;
	//	inputNext = -1;
	//}

	if (inputNext == -1) return;

	// ���� �Է� ���� ��� ���� Ŭ������
	BlendEdge* edge = nodes[currClip]->EdgeByNextClip(inputNext);
	if (edge != nullptr)
		PlayNextClip(edge, false);

	inputNext = -1;
}

void Animator::PlayNextClip(BlendEdge * next, bool bTemp)
{
	if (bTempEdge)
		SafeDelete(currEdge);

	currEdge = next;

	for (AnimNotify& on : nodes[currClip]->OnChanges)
		on();

	ClipNode* nextClip = nodes[currEdge->End];
	nextClip->Timer.PlayOnce(nextClip->Speed);
	//nextClip->Timer.PlayLoop(nextClip->Speed);

	bTempEdge = bTemp;
	blendAlpha = 0.0f;

	if (next->TweenTime == 0)
		EndBlend();
}

void Animator::EndBlend()
{
	currClip = currEdge->End;
	currEdge = nullptr;
	blendAlpha = 0.0f;
}
