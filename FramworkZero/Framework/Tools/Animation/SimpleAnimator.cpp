#include "Framework.h"
#include "SimpleAnimator.h"

SimpleAnimator::SimpleAnimator()
{
}

SimpleAnimator::~SimpleAnimator()
{
}

void SimpleAnimator::Play(UINT value)
{
	nextState = value;
}

void SimpleAnimator::Update()
{
	if (currState != nextState)
	{
		nodes[nextState].Play();

		currState = nextState;
	}
}

void SimpleAnimator::AddNode(function<void(void)> funcPlay)
{
	nodes.push_back(
		{
			nodes.size(),
			funcPlay
		}
	);
}
