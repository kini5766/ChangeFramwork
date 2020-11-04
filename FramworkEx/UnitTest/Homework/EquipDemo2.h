#pragma once

#include "Systems/IExecute.h"

class EquipDemo2 : public IExecute
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
	class EquippableKachujin* kachujin = nullptr;
	UINT clip = 0;
	UINT item = 0;
};

// ����
// 4. ���� ��ü
// ��Ʈ
// �ִ��� ���ͷ�Ʈ �߻����ϰ�
// �̸� ���⺰�� srv�� ����

// ���
// 1 unarmed (Į�ֱ�)
// 2 get (Į�̱�)
// 3 choice (Į����)
