#pragma once

class ClipTransformMap
{
public:
	ClipTransformMap(Model* model);
	ClipTransformMap(ModelClip** pClips, UINT clipCount, ModelBone** pBones, UINT boneCount);
	~ClipTransformMap();
	void Initialize(ModelClip** pClips, ModelBone** pBones, UINT boneCount);

public:
	ID3D11Texture2D* GetMap() { return texture; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

private:
	struct ClipTransform;
	void CreateTexture(ClipTransform** clipTransforms,  UINT boneCount, UINT maxFrameCount);
	void CreateSrv();

private:
	ID3D11Texture2D* texture = nullptr;
	UINT clipCount;
	ID3D11ShaderResourceView* srv = nullptr;

private:
	struct ClipTransform
	{
		UINT FrameCount = 0;
		UINT BoneCount = 0;

		Matrix** TransformArr2D;

		ClipTransform(UINT frameCount, UINT boneCount);
		~ClipTransform();
		void SetClipTransform(ModelBone** bones, UINT boneCount, ModelClip * clip);
	};
};