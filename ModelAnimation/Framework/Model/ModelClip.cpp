#include "Framework.h"
#include "ModelClip.h"


#pragma region Clipkeys

// ClipVectorKey ////////////////////////////////////////////////////////////////////////////

ClipVectorKey::ClipVectorKey(const vector<KeyVector>* keyVectors)
	: keyVectors(keyVectors)
{
}

ClipVectorKey::~ClipVectorKey()
{
}

void ClipVectorKey::Reset()
{
	currFrame = 0;
	runTime = 0.0f;
	bPlaying = false;
}

void ClipVectorKey::Start()
{
	if ((*keyVectors).size() > 1)
		bPlaying = true;
}

void ClipVectorKey::Stop()
{
	bPlaying = false;
}

void ClipVectorKey::Update(float deltaTime)
{
	if (!bPlaying)
		return; // 정지중

	runTime += deltaTime;

	// 다음 프레임으로
	while (runTime > (*keyVectors)[currFrame].Time)
	{
		runTime -= (*keyVectors)[currFrame].Time;
		if (++currFrame + 1 == (*keyVectors).size())
		{
			// 마지막 프레임
			runTime = 0.0f;
			runTime = (*keyVectors)[currFrame].Time;
			bPlaying = false;
			return;
		}
	}
}

void ClipVectorKey::GetVector(Vector3* out)
{
	(*out) = (*keyVectors)[currFrame].Value;
	if (runTime == 0.0f) return;

	const Vector3* nextValue = &(*keyVectors)[currFrame + 1].Value;
	float t = runTime / (*keyVectors)[currFrame].Time;

	D3DXVec3Lerp(out, out, nextValue, t);
}


// ClipRotationKey ////////////////////////////////////////////////////////////////////////////

ClipRotationKey::ClipRotationKey(const vector<KeyQuat>* keyQuats)
	: keyQuats(keyQuats)
{
}

ClipRotationKey::~ClipRotationKey()
{
}

void ClipRotationKey::Reset()
{
	currFrame = 0;
	runTime = 0.0f;
	bPlaying = false;
}

void ClipRotationKey::Start()
{
	if ((*keyQuats).size() > 1)
		bPlaying = true;
}

void ClipRotationKey::Stop()
{
	bPlaying = false;
}

void ClipRotationKey::Update(float deltaTime)
{
	if (!bPlaying)
		return; // 정지중

	runTime += deltaTime;

	// 다음 프레임으로
	while (runTime > (*keyQuats)[currFrame].Time)
	{
		runTime -= (*keyQuats)[currFrame].Time;

		if (++currFrame + 1 == (*keyQuats).size())
		{
			// 마지막 프레임
			runTime = 0.0f;
			runTime = (*keyQuats)[currFrame].Time;
			bPlaying = false;
			return;
		}
	}
}

void ClipRotationKey::GetRotaion(Quaternion* out)
{
	(*out) = (*keyQuats)[currFrame].Value;
	if (runTime == 0.0f) return;

	const Quaternion* nextValue = &(*keyQuats)[currFrame + 1].Value;
	float t = runTime / (*keyQuats)[currFrame].Time;

	D3DXQuaternionSlerp(out, out, nextValue, t);
}

#pragma endregion


#pragma region ClipBone

ClipBone::ClipBone(const ClipBoneData * data)
	: name(data->BoneName)
{
	keyPositions = new ClipVectorKey(&data->KeyPositions);
	keyRotations = new ClipRotationKey(&data->KeyRotations);
	keyScales = new ClipVectorKey(&data->KeyScales);
}

ClipBone::~ClipBone()
{
	SafeDelete(keyPositions);
	SafeDelete(keyRotations);
	SafeDelete(keyScales);
}

void ClipBone::Reset()
{
	keyPositions->Reset();
	keyRotations->Reset();
	keyScales->Reset();
}

void ClipBone::Start()
{
	keyPositions->Start();
	keyRotations->Start();
	keyScales->Start();
}

void ClipBone::Stop()
{
	keyPositions->Stop();
	keyRotations->Stop();
	keyScales->Stop();
}

void ClipBone::Update(float deltaTime)
{
	keyPositions->Update(deltaTime);
	keyRotations->Update(deltaTime);
	keyScales->Update(deltaTime);
}

void ClipBone::Position(Vector3 * out)
{
	keyPositions->GetVector(out);
}

void ClipBone::Rotation(Quaternion * out)
{
	keyRotations->GetRotaion(out);
}

void ClipBone::Scale(Vector3 * out)
{
	keyScales->GetVector(out);
}

#pragma endregion


#pragma region ClipModel

ClipModel::ClipModel(ClipData* data)
	: data(data)
{
	for (ClipBoneData* bone : data->Bones)
		bones.push_back(new ClipBone(bone));
}

ClipModel::~ClipModel()
{
	for (auto d : bones)
		SafeDelete(d);
}

void ClipModel::Reset()
{
	runningTime = 0.0f;
	for (auto bone : bones)
		bone->Reset();
}

void ClipModel::Start(float time)
{
	for (auto bone : bones)
	{
		bone->Start();
		bone->Update(time);
	}
}

void ClipModel::Stop()
{
	for (auto bone : bones)
		bone->Stop();
}

void ClipModel::Update()
{
	float deltaTime = Time::Delta() * speed * data->FrameRate;
	runningTime += deltaTime;
	for (auto bone : bones)
		bone->Update(deltaTime);

	float dt = runningTime - data->Duration;
	if (dt > 0.0f)
	{
		Reset();
		Start(dt);
	}
}

ClipBone * ClipModel::BoneByName(wstring & value)
{
	for (ClipBone* bone : bones)
	{
		if (bone->Name() == value)
		{
			return bone;
		}
	}
	return nullptr;
}

#pragma endregion


#pragma region ModelAnimationEx

using namespace ShaderEffctConstantName;

ModelAnimationEx::ModelAnimationEx(vector<ModelBone*>& _bones, vector<ClipData*>& datas)
{
	for (ClipData* data : datas)
		clips.push_back(new ClipModel(data));

	for (auto bone : _bones)
	{
		BindedKey* key = new BindedKey();
		key->Bone = bone;
		for (ClipModel* clip : clips)
			key->ClipNumToKey.push_back(clip->BoneByName(bone->Data()->Name));
		bindedKeys.push_back(key);
	}

	buffer = new ConstantBuffer(&skinningModelDesc, sizeof(SkinningModelDesc));
}

ModelAnimationEx::~ModelAnimationEx()
{
	for (auto d : bindedKeys)
		SafeDelete(d);

	for (auto d : clips)
		SafeDelete(d);
}

void ModelAnimationEx::Update()
{
	UpdateTweening();
}

void ModelAnimationEx::Render()
{
	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}


void ModelAnimationEx::PlayClip(UINT clip, float speed, float takeTime)
{
	takeTimeDiv = 1.0f / takeTime;
	tweenTime = 0.0f;
	next = clip;
	clips[next]->Speed(speed);
	clips[next]->Reset();
	clips[next]->Start();
}

float ModelAnimationEx::GetClipLength(UINT clip)
{
	return clips[clip]->Duration();
}

float ModelAnimationEx::CurrRunningTime()
{
	return clips[curr]->RunningTime();
}

void ModelAnimationEx::SetShader(Shader * shader)
{
	sBuffer = shader->AsConstantBuffer(CB_SkinningModel);
}


void ModelAnimationEx::UpdateTweening()
{
	if (curr == -1)
	{
		if (next == -1)
			return;  // 재생할 클립 없음

		// 정지 중 실행
		curr = next;
		next = -1;
	}

	TweeningBone();

	clips[curr]->Update();

	UpdateTweenTime();
}

void ModelAnimationEx::TweeningBone()
{
	UINT size = bindedKeys.size();
	for (UINT i = 0; i < size; i++)
	{
		ModelBone* bone = bindedKeys[i]->Bone;
		ClipBone* clipCurr = bindedKeys[i]->ClipNumToKey[curr];
		ClipBone* clipNext = nullptr;
		if (next != -1)
			clipNext = bindedKeys[i]->ClipNumToKey[next];

		BlendBone(bone, clipCurr, clipNext, tweenTime);

		Matrix inv;
		D3DXMatrixInverse(&inv, nullptr, &bone->Data()->Transform);

		Matrix boneT;
		bone->GetTransform()->LossyWorld(&boneT);

		skinningModelDesc.SkinningBoneTransforms[i] = inv * boneT;
	}
}

void ModelAnimationEx::UpdateTweenTime()
{
	if (next != -1)
	{
		tweenTime += Time::Delta() * takeTimeDiv;
		clips[next]->Update();

		if (tweenTime >= 1.0f)
		{
			clips[curr]->Stop();
			tweenTime = 0.0f;

			curr = next;
			next = -1;
		}
	}
}

void ModelAnimationEx::BlendBone(ModelBone * bone, ClipBone * clipCurr, ClipBone * clipNext, float alpha)
{
	if (clipCurr != nullptr)
	{
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		clipCurr->Position(&position);
		clipCurr->Rotation(&rotation);
		clipCurr->Scale(&scale);

		if (clipNext != nullptr)
		{
			Vector3 position2;
			Quaternion rotation2;
			Vector3 scale2;

			clipNext->Position(&position2);
			clipNext->Rotation(&rotation2);
			clipNext->Scale(&scale2);

			D3DXVec3Lerp(&position, &position, &position2, alpha);
			D3DXQuaternionSlerp(&rotation, &rotation, &rotation2, alpha);
			D3DXVec3Lerp(&scale, &scale, &scale2, alpha);
		}

		bone->GetTransform()->Position(position);
		bone->GetTransform()->Rotation(rotation);
		bone->GetTransform()->Scale(scale);
	}
}

#pragma endregion
