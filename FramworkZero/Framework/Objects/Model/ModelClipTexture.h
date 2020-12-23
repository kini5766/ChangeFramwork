#pragma once

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

private:
	struct ClipTransform  // Todo
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
};