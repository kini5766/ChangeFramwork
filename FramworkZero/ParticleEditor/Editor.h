#pragma once
#include "Systems/IExecute.h"

class Editor : public IExecute
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
	// 파티클 폴더 안의 .xml 찾기
	void UpdateParticleList();
	void UpdateTextureList();

private:
	void OnGui();
	void OnGui_List();
	void OnGui_Settings();
	void OnGui_Write();
	void WriteFile(wstring file);  // xml 저장

private:
	float windowWidth = 500;
	bool bLoop = false;
	UINT maxParticle = 0;
	vector<wstring> particleList;
	vector<wstring> textureList;
	wstring file = L"";

private:
	Particle* particle = nullptr;
	Shader* shaderMesh;
	CubeSky* sky;
	Mesh* sphere;
	Mesh* grid;

	vector<Mesh*> meshes;
};
