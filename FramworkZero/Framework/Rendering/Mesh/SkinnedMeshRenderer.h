#pragma once

struct SkinnedMeshData
{
	vector<Matrix*> MatrixBones;
	vector<MeshData*> Meshes;
	vector<Material*> Materials;
};

class SkinnedMeshRenderer
{
public:
	SkinnedMeshRenderer(SkinnedMeshData* desc);
	~SkinnedMeshRenderer();

public:
	void Render();
	void RenderInstance(UINT drawCount);

public:
	UINT RendererCount() const { return renderers.size(); }
	void Pass(UINT value);

private:
	vector<MeshRenderer*> renderers;
	ID3D11Texture2D* invBindPose;
	ID3D11ShaderResourceView* srvInvBindPose;
};

struct MeshBindPose
{
	ID3D11ShaderResourceView* SrvInvBindPose = nullptr;
	ID3D11ShaderResourceView* SrvBonesMap = nullptr;
};
