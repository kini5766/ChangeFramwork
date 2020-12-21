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
	// (�ִϸ��̼�out) ���� ��*�ν��Ͻ� texture
	ID3D11ShaderResourceView* srvLocalBones = nullptr;

	ShaderSetter* computeShader;
	// In : ��World
	// In : (�ִϸ��̼�out) ���� ��*�ν��Ͻ� texture
	// In : inv ��, �� �θ� �ε�����
	StructuredBuffer* computeBoneDescBuffer;
	// Out : ���� ��*�ν��Ͻ� texture
	TextureBuffer* computeOutputBoneBuffer;
	// Out : Skinned�����ų ��*�ν���Ʈ texture
	TextureBuffer* computeOutputSrvBuffer;

private:
	struct BoneDesc
	{
		// �� �θ� �ε���
		int Parent;
		// inv ��
		Matrix InvBone;
	}* boneDesc;
};