#pragma once
#include "Systems/IExecute.h"

class ExportFileDemo : public IExecute
{
public:
	// IExecute��(��) ���� ��ӵ�
	void Initialize() override;
	void Ready() override {}
	void Destroy() override {}
	void Update() override {}
	void PreRender() override {}
	void Render() override {}
	void PostRender() override {}
	void ResizeScreen() override {}

private:
	void Airplane();
	void Tower();
	void Tank();
	void Kachujin();
	void Paladin();
	void Weapon();
};
