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
}

void Animator::PlayTempBlend(UINT next, float tweenTime)
{
	BlendEdge* edge = new BlendEdge();
	edge->Start = currClip;
	edge->End = next;
	edge->TweenTime = tweenTime;
	PlayNextClip(edge, true);
}

void Animator::Update()
{
	// 트위닝 중 일 때
	if (currEdge != nullptr)
		UpdateTweening();

	// 트위닝 중이 아닐 때
	else NextInput();

	// 클립 업데이트
	UpdateClip();
}

void Animator::AddNode(float duration, float speed, float frameRate, function<void(void)> funcPlay)
{
	ClipNode* node = new ClipNode();
	node->ClipNum = nodes.size();
	node->Play = funcPlay;
	node->Time.SetClip(node->ClipNum, duration, frameRate);
	node->Speed = speed;
	nodes.push_back(node);

	BlendEdge* edge = new BlendEdge();
	edge->Start = node->ClipNum;
	edge->End = node->ClipNum;
	edge->TweenTime = 0.0f;
	node->Edges.push_back(edge);
	node->DefaultEdge = edge;

	// 처음 시작할 재생 번호
	if (node->ClipNum == currClip)
	{
		ClipNode* curr = nodes[currClip];
		curr->Play();
		curr->Time.PlayOnce(curr->Speed);
	}

}

void Animator::AddBlendEdge(UINT start, UINT end, float tweenTime, bool bDefault)
{
	BlendEdge* edge = nodes[currClip]->EdgeByNextClip(inputNext);
	if (edge == nullptr) edge = new BlendEdge();
	edge->TweenTime = tweenTime;
	nodes[start]->Edges.push_back(edge);

	if (bDefault)
		nodes[start]->DefaultEdge = edge;
}

void Animator::GetAnimDesc(KeyframeDesc * outDesc)
{
	outDesc->Clip = currClip;
	outDesc->Time = nodes[currClip]->Time.RunningTime();
}

void Animator::UpdateTweening()
{
	blendAlpha += Time::Delta() / currEdge->TweenTime;

	// 트위닝 끝
	if (blendAlpha >= 1.0f)
		EndTween();
}

void Animator::UpdateClip()
{
	// 클립 업데이트
	ClipNode* curr = nodes[currClip];
	curr->Time.Update();

	BlendEdge* edge = curr->DefaultEdge;
	float startTime = curr->Time.Duration();
	startTime -= curr->Time.FrameRate() * edge->TweenTime;

	// 클립 재생시간 끝났는 데도 입력이 없을 경우
	if (curr->Time.RunningTime() >= startTime)
		PlayNextClip(edge, false);
}

void Animator::NextInput()
{
	if (inputNext == -1) return;

	// 다음 입력 있을 경우 다음 클립으로
	BlendEdge* edge = nodes[currClip]->EdgeByNextClip(inputNext);
	if (edge != nullptr)
	{
		PlayNextClip(edge, false);
		inputNext = -1;
	}
}

void Animator::PlayNextClip(BlendEdge * next, bool bTemp)
{
	if (bTempEdge)
		SafeDelete(currEdge);

	currEdge = next;

	ClipNode* nextClip = nodes[currEdge->End];
	nextClip->Play();
	nextClip->Time.PlayOnce(nextClip->Speed);

	bTempEdge = bTemp;

	if (next->TweenTime == 0)
		EndTween();
}

void Animator::EndTween()
{
	currClip = currEdge->End;
	currEdge = nullptr;
	blendAlpha = 0.0f;
}
