#pragma once


#pragma region ModelAnimMap

class ModelAnimMap
{
public:
	// Ű������ �� �ִ�ġ (1Ŭ��-1�� �߿�)
	// �ּҰ� �ƴ� ���� �Ѱܾ��ϱ� ������ 1�������� ����
	struct KeyframeCount
	{
		UINT Translation;  // r
		UINT Rotaion;  // g
		UINT Scale;  // b
		UINT MaxCount;  // a
	};

public:
	ModelAnimMap(ModelData* data);
	~ModelAnimMap();

	ID3D11ShaderResourceView* ClipBoneMapSrv();
	ID3D11ShaderResourceView* FrameCountsSrv() { return computeCountBuffer->SRV(); }

private:
	// Ŭ��Ʈ������
	class ModelClipTexture* clipBoneMap;
	Texture2D* frameCountsTexture;
	TextureBuffer* computeCountBuffer;
	// keyframeCount[clipIndex * boneCount + boneIndex]
	//KeyframeCount* keyframeCounts;
};

#pragma endregion


#pragma region ModelClipTexture

// Ŭ��Ʈ������
// x��
// bone * 4 + 0 : Translation
// bone * 4 + 1 : Rotaion
// bone * 4 + 2 : Scale
// bone * 4 + 3 : Frame Time
// y�� : Ű ������ ��ȣ
// �迭 : Ŭ�� ��ȣ
class ModelClipTexture
{
public:
	ModelClipTexture(ModelData* data);
	~ModelClipTexture();

public:
	ID3D11ShaderResourceView* GetSRV() { return texture->GetSRV(); }

private:
	Texture2D* texture;
};

#pragma endregion


#pragma region ClipTransform

struct ClipTransform
{
public:
	ClipTransform();
	~ClipTransform();

	void NewArray(UINT boneCount, UINT maxFrameCount);
	void SetData(ClipBoneData** data, UINT BonesSize);
	void* GetData(UINT height);

private:
	UINT boneCount;
	UINT maxFrameCount;
	// Translation : 11 12 13 14
	// Rotation : 21 22 23 24
	// Scale : 31 32 33 34
	// FrameTime : 41 42 43 44
	// datas[maxFrameCount][boneCount]
	Matrix** datas;
};

#pragma endregion

