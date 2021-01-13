#pragma once

constexpr auto MODEL_INSTANCE_MAX_COUNT = 64;

struct ModelDesc
{
	wstring MeshFile;
	wstring MaterialFile;
	vector<wstring> ClipFiles;
};

struct VertexModel
{
	Vector3 Position;
	Vector2 Uv;
	Vector3 Normal;
	Vector3 Tangent;  // NormalMapping
	Vector4 BlendIndices;  // Animation Skinning BoneIndex
	Vector4 BlendWeight;  // Animation Skinning ����ġ

	VertexModel()
	{
		Position = Vector3(0, 0, 0);
		Uv = Vector2(0, 0);
		Normal = Vector3(0, 0, 0);
		Tangent = Vector3(0, 0, 0);
		BlendIndices = Vector4(0, 0, 0, 0);
		BlendWeight = Vector4(0, 0, 0, 0);
	}
};


// Bone
struct ModelBoneData
{
	int Index;
	wstring Name;

	int ParentIndex;
	Matrix Transform;
};

// Mesh
struct ModelMeshData
{
	wstring Name;
	UINT BoneIndex;
	//vector<wstring> MaterialNames;
	MeshData* Mesh;
};

// Clip
struct KeyVector
{
	Vector3 Value;
	float Time;
};

struct KeyQuat
{
	Quaternion Value;
	float Time;
};

struct ClipBoneData
{
	vector<KeyVector> KeyTranslations;
	vector<KeyQuat> KeyRotations;
	vector<KeyVector> KeyScales;
	//UINT TranslationCount;
	//UINT RotaionCount;
	//UINT ScaleCount;

	// ���� �� ���� ��� MaxFrame == 0
	// MeshBone�� ������ AnimBone�� ���� ��� -> MeshBone �״�� ���
	UINT MaxFrame;
};

struct ModelClipData
{
	wstring Name;
	float FrameRate;
	float Duration;
	vector<ClipBoneData*> Bones;  // (�޽��� : ���׺�)=(1 : 1) �ε��� ��Ī
};