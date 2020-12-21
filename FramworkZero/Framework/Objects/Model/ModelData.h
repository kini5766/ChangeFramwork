#pragma once

constexpr auto MODEL_INSTANCE_MAX_COUNT = 64;

struct VertexModel;
struct ModelMeshData;
struct ModelBoneData;

struct ModelDesc
{
	wstring MeshFile;
	wstring MaterialFile;
	vector<wstring> ClipFiles;
};

class ModelData
{
public:
	ModelData();
	~ModelData();

public:
	UINT BoneCount() { return bones.size(); }
	vector<ModelBoneData*>& Bones() { return bones; }
	ModelBoneData* BoneByIndex(UINT index) { return bones[index]; }
	//ModelBoneData* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMeshData*>& Meshes() { return meshes; }
	ModelMeshData* MeshByIndex(UINT index) { return meshes[index]; }

	UINT MaterialCount() { return materials.size(); }
	vector<Material*>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	//Material* MaterialByName(wstring name);

private:
	vector<ModelMeshData*> meshes;
	vector<ModelBoneData*> bones;
	vector<Material*> materials;
};

struct VertexModel
{
	Vector3 Position;
	Vector2 Uv;
	Vector3 Normal;
	Vector3 Tangent;  // NormalMapping
	Vector4 BlendIndices;  // Animation Skinning BoneIndex
	Vector4 BlendWeight;  // Animation Skinning °¡ÁßÄ¡

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


struct ModelMeshData
{
	wstring Name;
	UINT BoneIndex;
	vector<wstring> MaterialNames;
	MeshData* Mesh;
};


struct ModelBoneData
{
	wstring Name;
	int ParentIndex;
	int Index;
	Matrix Transform;
};


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
	UINT TranslationCount;
	UINT RotaionCount;
	UINT ScaleCount;
	UINT MaxFrame;
	wstring BoneName;
};

struct ModelClipData
{
	wstring Name;
	float FrameRate;
	float Duration;
	vector<ClipBoneData*> Bones;
};