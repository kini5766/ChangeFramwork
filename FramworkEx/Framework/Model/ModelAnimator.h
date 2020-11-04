#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	// �ɹ� ���� �Լ�
public:
	void Update();
	void Render();

public:
	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 1.0f);

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);

public:
	Model* GetModel() { return model; }
	Transform* GetTransform() { return transform; }
	void Pass(UINT value);

	// �ɹ� ���� �Լ�
private:
	void Apply();

	// �ɹ� ����
private:
	Model* model;
	vector<class ModelMeshClipMap*> renderers;

	Shader* shader;
	Transform* transform;

	class ModelAnimation* animation = nullptr;
	class ModelClipMap* clipMap = nullptr;
};

// cBuffer�� �ִ�� ���� �� �ִ� ����Ʈ 4096
// ������ texture�� �¿��

// texture2D �迭��ȣ : Ŭ����ȣ
// x�� : ����ȣ
// y�� : ������


// ����
// �������� �ּ��ؿ���