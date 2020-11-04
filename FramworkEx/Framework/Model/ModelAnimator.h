#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	// 맴버 공용 함수
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

	// 맴버 개인 함수
private:
	void Apply();

	// 맴버 변수
private:
	Model* model;
	vector<class ModelMeshClipMap*> renderers;

	Shader* shader;
	Transform* transform;

	class ModelAnimation* animation = nullptr;
	class ModelClipMap* clipMap = nullptr;
};

// cBuffer로 최대로 보낼 수 있는 바이트 4096
// 넘으면 texture로 태우기

// texture2D 배열번호 : 클립번호
// x축 : 본번호
// y축 : 프레임


// 과제
// 집에가서 주석해오기