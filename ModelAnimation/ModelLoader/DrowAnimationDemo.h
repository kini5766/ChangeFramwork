#pragma once
#include "Systems/IExecute.h"

class DrowAnimationDemo : public IExecute
{
public:
	// IExecute��(��) ���� ��ӵ�
	void Initialize() override;
	void Ready() override {}
	void Destroy() override;
	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}
	void ResizeScreen() override {}

private:
	void Kachujin();
	void Paladin();

private:
	Shader* shader;

	ModelAnimator* kachujin = nullptr;
	ModelAnimator* paladin = nullptr;
	//Model* weapon = nullptr;
};

// ���� ��
// 1. orbit
// 2. model editor
// 3. animation run

// LU Decompose atan2() ����

// ����
// 4. ���� ��ü
// ��Ʈ
// �ִ��� ���ͷ�Ʈ �߻����ϰ�
// �̸� ���⺰�� srv�� ����

// ���
// 1 unarmed (Į�ֱ�)
// 2 get (Į�̱�)
// 3 choice (Į����)
