#pragma once

class SimpleAnimator
{
public:
	SimpleAnimator();
	~SimpleAnimator();

public:
	void Play(UINT value);
	void Update();

public:
	void AddNode(function<void(void)> funcPlay);
	UINT Curr() { return currState; }

private:
	struct ClipNode
	{
		// 클립 번호
		UINT ClipNum;
		// 재생
		function<void(void)> Play;
	};

	struct BlendEdge { };

	vector<ClipNode> nodes;
	UINT currState = 0;
	UINT nextState = 0;
};