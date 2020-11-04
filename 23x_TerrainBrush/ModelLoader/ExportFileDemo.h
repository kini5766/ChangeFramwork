#pragma once
#include "Systems/IExecute.h"

class ExportFileDemo : public IExecute
{
public:
	// IExecute을(를) 통해 상속됨
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
