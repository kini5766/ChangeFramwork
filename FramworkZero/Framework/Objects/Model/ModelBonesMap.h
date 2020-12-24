#pragma once

class ModelBonesMap
{
public:
	ModelBonesMap(ModelData* data);
	~ModelBonesMap();

	ID3D11ShaderResourceView* SRV() { return computeBoneDescBuffer->SRV(); }

private:
	StructuredBuffer* computeBoneDescBuffer;

private:
	struct BoneDesc
	{
		// 본 부모 인덱스
		int Parent;
		// inv 본
		//Matrix InvBone;
		// 에닝본 없을 시 적용 시킬 본
		Matrix DefaultBone;
	}*boneDesc;
};