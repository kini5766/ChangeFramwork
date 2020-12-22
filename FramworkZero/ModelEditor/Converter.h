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
	bool FoundMaterialData(aiMaterial* _material);  // �� ���� ���͸��� ����
	void WriteMaterialData(wstring savePath);  // Xml���Ϸ� ��ȯ
	string WriteTexture(wstring saveFolder, string _file);  // �ؽ��� ����

#pragma endregion


#pragma region ExportAnimClip

public:
	// fbx�� ����(Ŭ��) ������ ������ �и�, ����
	void GetClipList(vector<wstring>* list);

	void ExportAnimClip(UINT index, wstring savePath);

private:
	struct asExClip* ReadClipData(aiAnimation* srcAnimation);

	void ReadKeyframeData(asExClip * clip, aiNode * node, vector<struct asExClipBone*>& aniBones);

	// ���Ϸ� ����
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