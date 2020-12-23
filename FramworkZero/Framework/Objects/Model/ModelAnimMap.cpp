#include "Framework.h"
#include "ModelAnimMap.h"


#pragma region ModelAnimMap

ModelAnimMap::ModelAnimMap(ModelData * data)
{
	UINT clipCount = data->ClipCount();
	UINT boneCount = data->BoneCount();

	clipBoneMap = new ModelClipTexture(data);

	frameCountsTexture = new Texture2D(boneCount, clipCount);

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

	frameCountsTexture->Format(4u * 4u, DXGI_FORMAT_R32G32B32A32_UINT);
	frameCountsTexture->SetColors(keyframeCounts);
	frameCountsTexture->CreateTexture();
	computeCountBuffer = new TextureBuffer(frameCountsTexture->GetTexture());
	SafeDeleteArray(keyframeCounts);

}

ModelAnimMap::~ModelAnimMap()
{
	SafeDelete(computeCountBuffer);
	SafeDelete(frameCountsTexture);
	SafeDelete(clipBoneMap);
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

	texture = new Texture2D(boneCount * 4, maxFrame, false, false, clipCount);

	// Ŭ���� �����
	UINT pageSize = texture->PageSize();
	UINT arraySize = texture->ArraySize();
	UINT widthSize = texture->WidthSize();
	UINT height = texture->Height();

	// �ּҰ� �ƴ� ���� �Ѱܾ��ϱ� ������ 1���� �����Ϳ� �� ����

	// ����޸�
	// MEM_RESERVE : ����(Ȯ��)
	// PAGE_READWRITE : �а��� ����
	void* p = VirtualAlloc(nullptr, pageSize * arraySize, MEM_RESERVE, PAGE_READWRITE);

	for (UINT c = 0; c < arraySize; c++)
	{
		UINT start = c * pageSize;

		for (UINT k = 0; k < height; k++)
		{
			// BYTE : 1����Ʈ, �����Ҷ����� 1����Ʈ�� �̵�
			// width * k * sizeof(Matrix) : 1�پ� (1�پ� ����)
			// start : ������ �����ּ�
			void* temp = (BYTE*)p + k * widthSize + start;

			// MEM_COMMIT : ���� ���
			VirtualAlloc(temp, widthSize, MEM_COMMIT, PAGE_READWRITE);
			memcpy(temp, clipTramsforms[c].GetData(k), widthSize);
		}
	}

	const void** datas = new const void*[arraySize];
	for (UINT c = 0; c < arraySize; c++)
	{
		datas[c] = (BYTE*)p + c * pageSize;  // �������� ���� �����ּ�
	}
	texture->SetColorsArray(datas);

	texture->CreateTextureArray();
	texture->CreateSRVArray();

	// �ӽ� ���� �Ҵ�����
	SafeDeleteArray(datas);
	SafeDeleteArray(clipTramsforms);
	// 0: ��ü �Ҵ�����
	VirtualFree(p, 0, MEM_RELEASE);
}

ModelClipTexture::~ModelClipTexture()
{
	SafeDelete(texture);
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

