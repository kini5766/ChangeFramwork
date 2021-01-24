#pragma once
#include "Systems/IExecute.h"

class Editor : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}


private:
	void OpenTerrain(wstring file);

private:
	Shader* shader;
	Shadow* shadow;
	ProjectionTexture* peojTexure;

	CubeSky* sky;

private:
	Terrain* terrain = nullptr;
	class Brush* brush = nullptr;
	class TerrainFile* menuFile = nullptr;
};
