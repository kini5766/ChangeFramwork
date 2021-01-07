#include "Framework.h"
#include "Animator.h"

#include "Objects/Model/ModelAnimation.h"

Animator::Animator()
{
}

Animator::Animator(ModelAnimation * modelAnim)
{
	UINT size = modelAnim->GetClipCount();

	for (UINT i = 0; i < size; i++)
	{
		const ModelClipData* clip = modelAnim->GetClipData(i);
		AddNode(clip->Duration, 1.0f, clip->FrameRate);
	}

	modelAnim->SetAnimator(bind(&Animator::GetAnimDesc, this, placeholders::_1));
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
	curr->Time.Update();

	if (currEdge != nullptr)
	{
		// ���� Ŭ�� ������Ʈ
		ClipNode* next = nodes[currEdge->End];
		next->Time.Update();
	}
	else
	{
		BlendEdge* edge = curr->DefaultEdge;
		float startTime = curr->Time.Duration();
		startTime -= curr->Time.FrameRate() * curr->Time.Speed() * edge->TweenTime;

		// Ŭ�� ����ð� ������ ���� �Է��� ���� ���
		if (curr->Time.RunningTime() >= startTime)
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
	node->Time.SetClip(node->ClipNum, duration, frameRate);
	node->Speed = speed;
	nodes.push_back(node);

	BlendEdge* edge = new BlendEdge();
	edge->Start = node->ClipNum;
	edge->End = node->ClipNum;
	edge->TweenTime = 0.0f;
	node->Edges.push_back(edge);
	node->DefaultEdge = edge;

	// ó�� ������ ��� ��ȣ
	if (node->ClipNum == currClip)
	{
		ClipNode* curr = nodes[currClip];
		funcNext(currClip);
		curr->Time.PlayOnce(curr->Speed);
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
	outDesc->Clip[0].Time = nodes[currClip]->Time.RunningTime();

	if (currEdge != nullptr)
	{
		outDesc->Clip[1].Clip = currEdge->End;
		outDesc->Clip[1].Time = nodes[currEdge->End]->Time.RunningTime();
		outDesc->Alpha = blendAlpha;
	}
	else
	{
		outDesc->Alpha = 0.0f;
	}
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

	ClipNode* nextClip = nodes[currEdge->End];
	nextClip->Time.PlayOnce(nextClip->Speed);
	//nextClip->Time.PlayLoop(nextClip->Speed);

	bTempEdge = bTemp;
	blendAlpha = 0.0f;

	if (next->TweenTime == 0)
		EndBlend();
}

void Animator::EndBlend()
{
	currClip = currEdge->End;
	funcNext(currClip);
	currEdge = nullptr;
	blendAlpha = 0.0f;
}
