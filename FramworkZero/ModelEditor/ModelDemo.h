#pragma once
#include "Systems/IExecute.h"

class ModelDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	void Tank();
	void Tower();
	void Airplane();

private:
	ModelInstancing* tank = nullptr;
	ModelInstancing* tower = nullptr;
	ModelInstancing* airplane = nullptr;
};
