#pragma once

// fbx �� -> ����ü ������ ���
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
	// �ִϸ��̼�
	void ClipList(vector<wstring>* list);  // ä�� �и� ����
	void ExportAnimClip(UINT index, wstring savePath, bool bOverWrite = true);

	// �ִ� �� �� ä�� �и� �غ���(���̴� ����� ��ٷο�)

private:
	// ���͸���
	void ReadMaterial();
	void WriteMaterial(wstring savePath, bool bOverWrite);
	string WriteTexture(string savePath, string file);

	// �޽�
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void WriteMeshData(wstring savePath, bool bOverWrite);

	// ��Ű�� �޽�
	void ReadSkinData();

private:
	// �ִϸ��̼�
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

// ��Ű�� �޽�
// Ű������ ��� �����ӿ� ���� ����
// �ְ� ����ġ 4���� ���̴���
// ����ġ���� 1�� ������ �� �ȴ�.

// ���̴� 4096����Ʈ �̻� �� �Ѿ