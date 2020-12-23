#pragma once

struct MeshBindPose;

class SkinnedMeshRenderer
{
public:
	SkinnedMeshRenderer();
	~SkinnedMeshRenderer();

public:
	void Update();
	void Render();
	void RenderInstance();
	void RenderInstance(UINT drawCount);

public:
	MeshBindPose* BindPose() { return bindPose; }
	vector<MeshRenderer*>& Renderers() { return renderers; }
	UINT RendererCount() const { return renderers.size(); }
	void Pass(UINT value);

	void SetMaterials(Material** mats, UINT count);
	void UpdateMaterials();

private:
	MeshBindPose* bindPose;
	vector<MeshRenderer*> renderers;
};


struct MeshBindPose
{
	//ID3D11UnorderedAccessView* UavBonesMap = nullptr;
	ID3D11ShaderResourceView* SrvBonesMap = nullptr;
};