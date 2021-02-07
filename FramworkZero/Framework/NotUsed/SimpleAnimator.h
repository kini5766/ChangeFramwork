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
		// Ŭ�� ��ȣ
		UINT ClipNum;
		// ���
		function<void(void)> Play;
	};

	struct BlendEdge { };

	vector<ClipNode> nodes;
	UINT currState = 0;
	UINT nextState = 0;
};