#pragma once

class ModelSkinnedInstance
{
public:
	ModelSkinnedInstance();
	~ModelSkinnedInstance();

public:
	void Update();

private:
	void CreateCompute();

private:
	ModelData* data;
	SkinnedMeshRenderer* renderer;
	Transform* transform;

	Matrix world;
	UINT boneCount;
	// (애니메이션out) 로컬 본*인스턴스 texture
	ID3D11ShaderResourceView* srvLocalBones = nullptr;

	ShaderSetter* computeShader;
	// In : 모델World
	// In : (애니메이션out) 로컬 본*인스턴스 texture
	// In : inv 본, 본 부모 인덱스들
	StructuredBuffer* computeBoneDescBuffer;
	// Out : 로직 본*인스턴스 texture
	TextureBuffer* computeOutputBoneBuffer;
	// Out : Skinned적용시킬 본*인스턴트 texture
	TextureBuffer* computeOutputSrvBuffer;

private:
	struct BoneDesc
	{
		// 본 부모 인덱스
		int Parent;
		// inv 본
		Matrix InvBone;
	}* boneDesc;
};