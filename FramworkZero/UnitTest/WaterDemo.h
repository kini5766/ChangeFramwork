#pragma once
#include "Systems/IExecute.h"

class WaterDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

private:
	void Mesh();
	void Airplane();
	void Kachujin();
	//void Weapon();

	void PointLights();
	void SpotLights();

	//void SetTransform(Transform* transform);

private:
	Shadow* shadow;
	Water* water;

	CubeSky* sky;
	Terrain* terrain;

	MeshInstancing* cube;
	MeshInstancing* cylinder;
	MeshInstancing* sphere;

	
	ModelInstancing* airplane;
	ModelInstancing* kachujin;
	//ModelInstancing* weapon;
	//Transform* weaponTransform;
};
