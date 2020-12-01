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
		bones[bone->BoneName] = new ClipBone(bone);
}

ClipModel::~ClipModel()
{
	for (auto d : bones)
		SafeDelete(d.second);

	//for (auto d : data->Bones)
	//	SafeDelete(d);
	//SafeDelete(data);
}

void ClipModel::Reset()
{
	runningTime = 0.0f;
	for (auto bone : bones)
		bone.second->Reset();
}

void ClipModel::Start(float time)
{
	for (auto bone : bones)
	{
		bone.second->Start();
		bone.second->Update(time);
	}
}

void ClipModel::Stop()
{
	for (auto bone : bones)
		bone.second->Stop();
}

void ClipModel::Update()
{
	float deltaTime = Time::Delta() * speed * data->FrameRate;
	runningTime += deltaTime;
	for (auto bone : bones)
		bone.second->Update(deltaTime);

	float dt = runningTime - data->Duration;
	if (dt > 0.0f)
	{
		Reset();
		Start(dt);
	}
}

ClipBone * ClipModel::GetBone(wstring name)
{
	if (bones.count(name) == 0)
		return nullptr;
	return bones[name];
}

#pragma endregion


#pragma region ModelAnimationEx

#include "ModelMesh.h"
using namespace ShaderEffctConstantName;

ModelAnimationEx::ModelAnimationEx(const vector<ModelBone*>& _bones, const vector<ClipData*> datas)
	: bones(_bones)
{
	//bones.assign(_bones.begin(), _bones.end());

	for (ClipData* data : datas)
		clips.push_back(new ClipModel(data));

	buffer = new ConstantBuffer(&skinningModelDesc, sizeof(SkinningModelDesc));
}

ModelAnimationEx::~ModelAnimationEx()
{
	for (auto d : clips)
		SafeDelete(d);
}

void ModelAnimationEx::Update()
{
	if (curr == nullptr)
	{
		if (next == nullptr)
			return;

		curr = next;
		next = nullptr;
	}


	UINT i = 0;
	for (ModelBone* bone : bones)
	{
		ClipBone* clipCurr = curr->GetBone(bone->Data()->Name);

		if (clipCurr != nullptr)
		{
			Vector3 position;
			Quaternion rotation;
			Vector3 scale;

			clipCurr->Position(&position);
			clipCurr->Rotation(&rotation);
			clipCurr->Scale(&scale);

			if (next != nullptr)
			{
				ClipBone* clipNext = next->GetBone(bone->Data()->Name);

				if (clipNext != nullptr)
				{
					Vector3 position2;
					Quaternion rotation2;
					Vector3 scale2;

					clipNext->Position(&position2);
					clipNext->Rotation(&rotation2);
					clipNext->Scale(&scale2);

					D3DXVec3Lerp(&position, &position, &position2, tweenTime);
					D3DXQuaternionSlerp(&rotation, &rotation, &rotation2, tweenTime);
					D3DXVec3Lerp(&scale, &scale, &scale2, tweenTime);
				}
			}

			bone->GetTransform()->Position(position);
			bone->GetTransform()->Rotation(rotation);
			bone->GetTransform()->Scale(scale);
		}

		Matrix inv;
		D3DXMatrixInverse(&inv, nullptr, &bone->Data()->Transform);

		Matrix boneT;
		bone->GetTransform()->LossyWorld(&boneT);

		skinningModelDesc.SkinningBoneTransforms[i] = inv * boneT;
		i++;
	}
	curr->Update();
	if (next != nullptr)
	{
		tweenTime += Time::Delta() * takeTimeDiv;
		next->Update();

		if (tweenTime >= 1.0f)
		{
			curr->Stop();
			tweenTime = 0.0f;

			curr = next;
			next = nullptr;
		}
	}
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
	next = clips[clip];
	next->Speed(speed);
	next->Reset();
	next->Start();
}

float ModelAnimationEx::GetClipLength(UINT clip)
{
	return clips[clip]->Duration();
}

float ModelAnimationEx::CurrRunningTime()
{
	return curr->RunningTime();
}

void ModelAnimationEx::SetShader(Shader * shader)
{
	sBuffer = shader->AsConstantBuffer(CB_SkinningModel);
}

#pragma endregion
