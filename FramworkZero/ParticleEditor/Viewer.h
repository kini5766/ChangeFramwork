#pragma once
#include "Systems/IExecute.h"

class Viewer : public IExecute
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
	void Meshes();
	void Pass(UINT pass);

private:
	Particle* particle;
	Shader* shader;
	CubeSky* sky;
	Mesh* sphere;
	Mesh* grid;

	vector<Mesh*> meshes;
};