#pragma once

class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);


#pragma region ExportMesh

public:
	void ExportMesh(wstring savePath);

private:  // node : bone
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int index);
	void ReadSkinData();
	void WriteMeshData(wstring savePath);

#pragma endregion


#pragma region ExportMaterial

public:
	void ExportMaterial(wstring savePath, bool bOverWrite = true);

private:
	void ReadMaterialData();
	bool FoundMaterialData(aiMaterial* _material);  // 안 쓰는 메터리얼 제외
	void WriteMaterialData(wstring savePath);  // Xml파일로 변환
	string WriteTexture(wstring saveFolder, string _file);  // 텍스쳐 복사

#pragma endregion


#pragma region ExportAnimClip

public:
	// fbx에 동작(클립) 여러개 있으면 분리, 저장
	void GetClipList(vector<wstring>* list);

	void ExportAnimClip(UINT index, wstring savePath);

private:
	struct asExClip* ReadClipData(aiAnimation* srcAnimation);

	void ReadKeyframeData(asExClip * clip, aiNode * node, vector<struct asExClipBone*>& aniBones);

	// 파일로 저장
	void WriteClipData(struct asExClip* clip, wstring savePath);

#pragma endregion


private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asBone*> bones;
	vector<struct asMesh*> meshes;
	vector<struct asMaterial*> materials;
};