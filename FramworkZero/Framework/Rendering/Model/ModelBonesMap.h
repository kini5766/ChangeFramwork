#pragma once

class ModelBonesMap
{
public:
	ModelBonesMap(const struct ModelAnimDesc& anim);
	~ModelBonesMap();

	ID3D11ShaderResourceView* SRV() { return computeBoneDescBuffer->SRV(); }

private:
	StructuredBuffer* computeBoneDescBuffer;

private:
	struct BoneDesc
	{
		// �� �θ� �ε���
		int Parent;
		// inv ��
		//Matrix InvBone;
		// ���׺� ���� �� ���� ��ų ��
		Matrix DefaultBone;
	}*boneDesc;
};