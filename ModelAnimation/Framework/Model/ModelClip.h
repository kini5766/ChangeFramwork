#pragma once

#define MAX_SKINNING_MODEL_TRANSFORMS 256

#pragma region Data

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
	vector<KeyVector> KeyPositions;
	vector<KeyQuat> KeyRotations;
	vector<KeyVector> KeyScales;
	wstring BoneName;
};

struct ClipData
{
	wstring ClipName;
	float FrameRate;
	float Duration;
	vector<ClipBoneData*> Bones;
};

#pragma endregion


#pragma region ClipKeys

// ClipVectorKey ////////////////////////////////////////////////////////////////////////////

class ClipVectorKey
{
public:
	ClipVectorKey(const vector<KeyVector>* keyVectors);
	~ClipVectorKey();

public:
	void Reset();
	void Start();
	void Stop();

	void Update(float deltaTime);

public:
	void GetVector(Vector3* out);

private:
	const vector<KeyVector>* keyVectors;
	UINT currFrame = 0;
	float runTime = 0.0f;
	bool bPlaying = false;
};


// ClipRotationKey ////////////////////////////////////////////////////////////////////////////

class ClipRotationKey
{
public:
	ClipRotationKey(const vector<KeyQuat>* keyQuats);
	~ClipRotationKey();

public:
	void Reset();
	void Start();
	void Stop();

	void Update(float deltaTime);

public:
	void GetRotaion(Quaternion* out);

private:
	const vector<KeyQuat>* keyQuats;
	UINT currFrame = 0;
	float runTime = 0.0f;
	bool bPlaying = false;
};

#pragma endregion


#pragma region ClipBone

// ���� ����� Ű ����
class ClipBone
{
public:
	ClipBone(const ClipBoneData* data);
	~ClipBone();

public:
	void Reset();
	void Start();
	void Stop();

	void Update(float deltaTime);

	void Position(Vector3* out);
	void Rotation(Quaternion* out);
	void Scale(Vector3* out);

	wstring Name() { return name; }

private:
	ClipVectorKey* keyPositions;
	ClipRotationKey* keyRotations;
	ClipVectorKey* keyScales;
	wstring name;
};

#pragma endregion


#pragma region ClipModel

// �ϳ��� Ŭ���� �ִ� ��Ű��
class ClipModel
{
public:
	ClipModel(ClipData* data);
	~ClipModel();

public:
	void Reset();
	void Start(float time = 0.0f);
	void Stop();

	void Update();

public:
	ClipData* Data() { return data; }
	ClipBone* BoneByName(wstring& value);
	void Speed(float value) { speed = value; }
	float Duration() { return data->Duration; }
	float RunningTime() { return runningTime; }

private:
	ClipData* data;

	vector<ClipBone*> bones;
	float runningTime = 0.0f;
	float speed = 1.0f;
};

#pragma endregion


#pragma region ModelAnimationEx

class ModelAnimationEx
{
public:
	ModelAnimationEx(vector<class ModelBone*>& bones, vector<ClipData*>& datas);
	~ModelAnimationEx();

public:
	void Update();
	void Render();

public:
	void PlayClip(UINT clip, float speed, float takeTime);
	float GetClipLength(UINT clip);
	float CurrRunningTime();
	void SetShader(Shader* shader);

private:
	vector<ClipModel*> clips;

private:
	void UpdateTweening();
	void TweeningBone();
	void UpdateTweenTime();

	// ���� �� ����
	void BlendBone(ModelBone* bone, ClipBone* clipCurr, ClipBone* clipNext, float alpha);

private:
	float takeTimeDiv = 1.0f;  // ������ ���̴� �ð�
	float tweenTime = 0.0f;  // ���̰� �ִ� �� �ð�

	// ���� Ŭ�� �ε��� -1�� ��� ���� �ƴ�
	int curr = -1;
	// ���� Ŭ�� �ε��� -1�� ��� ���� �ƴ�
	int next = -1;
	
	ConstantBuffer* buffer = nullptr;
	ID3DX11EffectConstantBuffer* sBuffer;

private:
	// ����
	struct SkinningModelDesc
	{
		Matrix SkinningBoneTransforms[MAX_SKINNING_MODEL_TRANSFORMS];
	}skinningModelDesc;

	// ���� ��Ű ����
	struct BindedKey
	{
		ModelBone* Bone;
		// Ŭ�� ��ȣ�� ��Ű ���
		vector<ClipBone*> ClipNumToKey;
	}; 
	vector<BindedKey*> bindedKeys;
};

#pragma endregion

