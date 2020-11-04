#pragma once

// fbx 씬 -> 구조체 변수에 담기
class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);

public:
	void ExportMaterial(wstring savePath, bool bOverWrite = true);
	void ExportMesh(wstring savePath, bool bOverWrite = true);

public:
	// 애니메이션
	void ClipList(vector<wstring>* list);  // 채널 분리 역할
	void ExportAnimClip(UINT index, wstring savePath, bool bOverWrite = true);

	// 애니 두 개 채널 분리 해보기(쉐이더 계산이 까다로움)

private:
	// 메터리얼
	void ReadMaterial();
	void WriteMaterial(wstring savePath, bool bOverWrite);
	string WriteTexture(string savePath, string file);

	// 메쉬
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void WriteMeshData(wstring savePath, bool bOverWrite);

	// 스키닝 메쉬
	void ReadSkinData();

private:
	// 애니메이션
	struct asClip* ReadClipData(aiAnimation* animation);
	void ReadKeyframeData(struct asClip* clip, aiNode* node, vector<struct asClipNode>& aiNodeInfos);
	void WriteClipData(struct asClip* clip, wstring savePath, bool bOverWrite);
	

private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asMaterial*> materials;
	vector<struct asBone*> bones;
	vector<struct asMesh*> meshes;
};

// 스키닝 메쉬
// 키프레임 모든 프레임에 전부 저장
// 최고 가중치 4개만 쉐이더로
// 가중치합이 1이 넘으면 안 된다.

// 쉐이더 4096바이트 이상 안 넘어감