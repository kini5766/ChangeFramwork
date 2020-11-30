#pragma once
#include "Systems/IExecute.h"

class DrowModelDemo : public IExecute
{
public:
	// IExecute을(를) 통해 상속됨
	void Initialize() override;
	void Ready() override {}
	void Destroy() override;
	void Update() override;
	void PreRender() override {}
	void Render() override;
	void PostRender() override {}
	void ResizeScreen() override {}

private:
	void Airplane();
	void Tower();
	void Tank();
	void Kachujin();

private:
	Shader* shader;

	ModelRender* airplane = nullptr;
	ModelRender* tower = nullptr;
	ModelRender* tank = nullptr;
	ModelRender* kachujin = nullptr;
};