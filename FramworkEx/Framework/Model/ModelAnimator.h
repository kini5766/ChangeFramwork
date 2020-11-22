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
	//vector<class ModelMeshClipMap*> renderers;
	vector<class ModelMesh*> renderers;
	class ModelClipMap* clipMap = nullptr;

private:
	Shader* shader;
	MaterialGroup* materials;
	Transform* transform;
	PerFrameBuffer* perFrame;
	class ModelAnimation* animation;
};

// cBuffer�� �ִ�� ���� �� �ִ� ����Ʈ 4096
// ������ texture�� �¿��

// texture2D �迭��ȣ : Ŭ����ȣ
// x�� : ����ȣ
// y�� : ������


// ����
// �������� �ּ��ؿ���