#pragma once

// ----------------------------------------------------------------------------
// ModelClip
// ----------------------------------------------------------------------------

struct ModelKeyframeData
{
	float Time;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

struct ModelKeyframe
{
	wstring BoneName;
	vector<ModelKeyframeData> Transforms;
};

class ModelClip
{
public: 
	friend class Model;

private:
	ModelClip();
	~ModelClip();

public:
	float Duration() { return duration; }
	float FrameRate() { return frameRate; }
	UINT FrameCount() { return frameCount; }

	// bone name -> ModelKeyframe
	ModelKeyframe* Keyframe(wstring name);

private:
	wstring name;

	float duration;
	float frameRate;
	UINT frameCount;

	unordered_map<wstring, ModelKeyframe*> keyframeMap;
};


// ----------------------------------------------------------------------------
// ModelClipMap
// ----------------------------------------------------------------------------

class ModelClipMap
{
public:
	ModelClipMap(Model* model);
	ModelClipMap(ModelClip** pClips, UINT clipCount, ModelBone** pBones, UINT boneCount);
	~ModelClipMap();
	void Initialize(ModelClip** pClips, ModelBone** pBones, UINT boneCount);

public:
	ID3D11Texture2D* GetMap() { return texture; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

private:
	struct ClipTransform;
	void CreateTexture(ClipTransform** clipTransforms, UINT boneCount, UINT maxFrameCount);
	void CreateSrv();

private:
	ID3D11Texture2D* texture = nullptr;
	UINT clipCount;
	ID3D11ShaderResourceView* srv = nullptr;

private:
	struct ClipTransform
	{
		UINT FrameCount = 0;
		UINT BoneCount = 0;

		Matrix** TransformArr2D;

		ClipTransform(UINT frameCount, UINT boneCount);
		~ClipTransform();
		void SetClipTransform(ModelBone** bones, UINT boneCount, ModelClip * clip);
	};
};


// ----------------------------------------------------------------------------
// AnimationClip
// ----------------------------------------------------------------------------

class ModelAnimation
{
public:
	ModelAnimation(ModelClip** clips, UINT clipCount);
	~ModelAnimation();

public:
	void UpdateNoTweening();
	void UpdateTweening();
	void Render();

public:
	void CreateBuffer(Shader* shader);
	void PlayClip(UINT clip, float speed, float takeTime);
	float GetClipLength(UINT clip);
	float GetClipRunTime();

private:
	struct KeyframeDesc;
	void UpdateClipLoop(KeyframeDesc& desc, ModelClip* clip);
	void NextClip();

private:
	ModelClip** clips;
	UINT clipCount;
	ConstantBuffer* frameBuffer = nullptr;
	ID3DX11EffectConstantBuffer* sFrameBuffer = nullptr;

private:
	struct KeyframeDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.0f;
		float RunningTime = 0.0f;

		float Speed = 1.0f;
		float Padding[2];
	}/*keyframeDesc*/;

	// 동작과 보간
	struct TweenDesc
	{
		float TakeTimeDiv = 1.0f;  // 동작이 섞이는 시간
		float TweenTime = 0.0f;  // 섞이고 있는 중 시간
		float RunningTime = 0.0f;
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;  // -1은 없다 간주
		}
	} tweenDesc;
};