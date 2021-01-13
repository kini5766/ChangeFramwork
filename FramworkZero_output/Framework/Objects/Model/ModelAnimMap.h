#pragma once


#pragma region ModelAnimMap

class ModelAnimMap
{
public:
	// 키프레임 별 최대치 (1클립-1본 중에)
	// 주소가 아닌 값을 넘겨야하기 때문에 1차원으로 선언
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
	// 클립트랜스폼
	class ModelClipTexture* clipBoneMap;
	Texture2D* frameCountsTexture;
	TextureBuffer* computeCountBuffer;
	// keyframeCount[clipIndex * boneCount + boneIndex]
	//KeyframeCount* keyframeCounts;
};

#pragma endregion


#pragma region ModelClipTexture

// 클립트랜스폼
// x축
// bone * 4 + 0 : Translation
// bone * 4 + 1 : Rotaion
// bone * 4 + 2 : Scale
// bone * 4 + 3 : Frame Time
// y축 : 키 프레임 번호
// 배열 : 클립 번호
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

