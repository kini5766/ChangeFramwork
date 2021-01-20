#include "Framework.h"
#include "ModelAnimMap.h"


#pragma region ModelAnimMap

ModelAnimMap::ModelAnimMap(ModelData * data)
{
	UINT clipCount = data->ClipCount();
	UINT boneCount = data->BoneCount();

	clipBoneMap = new ModelClipTexture(data);

	KeyframeCount* keyframeCounts = new KeyframeCount[clipCount * boneCount];
	for (UINT c = 0; c < clipCount; c++)
	{
		UINT boneSize = data->ClipByIndex(c)->Bones.size();
		for (UINT b = 0; b < boneSize; b++)
		{
			KeyframeCount& k = keyframeCounts[c * boneCount + b];
			ClipBoneData* bone = data->Clips()[c]->Bones[b];
			k.Translation = bone->KeyTranslations.size();
			k.Rotaion = bone->KeyRotations.size();
			k.Scale = bone->KeyScales.size();
			k.MaxCount = bone->MaxFrame;
		}
	}

	Texture2DDesc tex2DDesc;
	D3D11_TEXTURE2D_DESC& desc = tex2DDesc.Desc();
	desc.Width = boneCount;
	desc.Height = clipCount;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;  // 쉐이더에 사용할 텍스쳐
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	tex2DDesc.SetColors(keyframeCounts);

	frameCountsTexture = tex2DDesc.CreateTexture(4u * 4u);
	computeCountBuffer = new TextureBuffer(frameCountsTexture);
	SafeDeleteArray(keyframeCounts);

}

ModelAnimMap::~ModelAnimMap()
{
	SafeDelete(computeCountBuffer);
	SafeDelete(clipBoneMap);

	SafeRelease(frameCountsTexture);
}

ID3D11ShaderResourceView * ModelAnimMap::ClipBoneMapSrv()
{
	return clipBoneMap->GetSRV();
}

#pragma endregion


#pragma region ModelClipTexture

ModelClipTexture::ModelClipTexture(ModelData * data)
{
	UINT clipCount = data->ClipCount();
	UINT boneCount = data->BoneCount();

	UINT maxFrame = 0;
	for (UINT c = 0; c < clipCount; c++)
	{
		UINT bonesSize = data->Clips()[c]->Bones.size();
		for (UINT b = 0; b < bonesSize; b++)
			maxFrame = max(maxFrame, data->Clips()[c]->Bones[b]->MaxFrame);
	}

	ClipTransform* clipTramsforms = new ClipTransform[clipCount];
	for (UINT c = 0; c < clipCount; c++)
	{
		clipTramsforms[c].NewArray(boneCount, maxFrame);
		UINT bonesSize = data->Clips()[c]->Bones.size();
		clipTramsforms[c].SetData(data->Clips()[c]->Bones.data(), bonesSize);
	}


	Texture2DDesc tex2DDesc(clipCount);
	D3D11_TEXTURE2D_DESC& desc = tex2DDesc.Desc();
	desc.Width = boneCount * 4;
	desc.Height = maxFrame;
	desc.ArraySize = clipCount;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;  // 쉐이더에 사용할 텍스쳐
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;


	// 클립맵 만들기
	UINT stride = 4u * 4u;
	UINT arraySize = desc.ArraySize;
	UINT widthSize = desc.Width * stride;
	UINT height = desc.Height;
	UINT pageSize = widthSize * height;

	// 주소가 아닌 값을 넘겨야하기 때문에 1차원 포인터에 값 복사

	// 가상메모리
	// MEM_RESERVE : 예약(확보)
	// PAGE_READWRITE : 읽고쓰기 권한
	void* p = VirtualAlloc(nullptr, pageSize * arraySize, MEM_RESERVE, PAGE_READWRITE);

	for (UINT c = 0; c < arraySize; c++)
	{
		UINT start = c * pageSize;

		for (UINT k = 0; k < height; k++)
		{
			// BYTE : 1바이트, 덧셈할때마다 1바이트씩 이동
			// width * k * sizeof(Matrix) : 1줄씩 (1줄씩 저장)
			// start : 페이지 시작주소
			void* temp = (BYTE*)p + k * widthSize + start;

			// MEM_COMMIT : 권한 얻기
			VirtualAlloc(temp, widthSize, MEM_COMMIT, PAGE_READWRITE);
			memcpy(temp, clipTramsforms[c].GetData(k), widthSize);
		}

		void* temp = (BYTE*)p + start;
		tex2DDesc.SetColorsArray(c, temp);
	}

	texture = tex2DDesc.CreateTextureArray(stride);
	srv = tex2DDesc.CreateSRVArray();

	// 임시 변수 할당해제
	SafeDeleteArray(clipTramsforms);
	// 0: 전체 할당해제
	VirtualFree(p, 0, MEM_RELEASE);
}

ModelClipTexture::~ModelClipTexture()
{
	SafeRelease(srv);
	SafeRelease(texture);
}

#pragma endregion


#pragma region ClipTransform

ClipTransform::ClipTransform()
{
}

ClipTransform::~ClipTransform()
{
	for (UINT i = 0; i < maxFrameCount; i++)
		SafeDeleteArray(datas[i]);
	SafeDeleteArray(datas);
}

void ClipTransform::NewArray(UINT _boneCount, UINT _maxFrameCount)
{
	boneCount = _boneCount;
	maxFrameCount = _maxFrameCount;

	datas = new Matrix*[maxFrameCount];
	for (UINT i = 0; i < maxFrameCount; i++)
	{
		datas[i] = new Matrix[boneCount];
		ZeroMemory(datas[i], sizeof(Matrix) * boneCount);
	}
}

void ClipTransform::SetData(ClipBoneData ** data, UINT BonesSize)
{
	for (UINT b = 0; b < BonesSize; b++)
	{
		UINT size;

		// Translation
		size = data[b]->KeyTranslations.size();
		for (UINT f = 0; f < size; f++)
		{
			Matrix& bone = datas[f][b];
			KeyVector& k = data[b]->KeyTranslations[f];
			bone._11 = k.Value.x;
			bone._12 = k.Value.y;
			bone._13 = k.Value.z;
			bone._41 = k.Time;
		}

		// Rotation
		size = data[b]->KeyRotations.size();
		for (UINT f = 0; f < size; f++)
		{
			Matrix& bone = datas[f][b];
			KeyQuat& k = data[b]->KeyRotations[f];
			bone._21 = k.Value.x;
			bone._22 = k.Value.y;
			bone._23 = k.Value.z;
			bone._24 = k.Value.w;
			bone._42 = k.Time;
		}

		// Scale
		size = data[b]->KeyScales.size();
		for (UINT f = 0; f < size; f++)
		{
			Matrix& bone = datas[f][b];
			KeyVector& k = data[b]->KeyScales[f];
			bone._31 = k.Value.x;
			bone._32 = k.Value.y;
			bone._33 = k.Value.z;
			bone._43 = k.Time;
		}
	}
}

void * ClipTransform::GetData(UINT height)
{
	return (void*)datas[height];
}

#pragma endregion

