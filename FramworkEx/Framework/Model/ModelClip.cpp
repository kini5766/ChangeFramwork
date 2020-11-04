#include "Framework.h"
#include "ModelClip.h"


// ----------------------------------------------------------------------------
// ModelClip
// ----------------------------------------------------------------------------

ModelClip::ModelClip()
{
}

ModelClip::~ModelClip()
{
	for (auto d : keyframeMap)
		SafeDelete(d.second);
}

ModelKeyframe * ModelClip::Keyframe(wstring name)
{
	if (keyframeMap.count(name) < 1)
		return nullptr;

	return keyframeMap[name];
}


// ----------------------------------------------------------------------------
// ModelClipMap
// ----------------------------------------------------------------------------
#include "ModelMesh.h"

ModelClipMap::ModelClipMap(Model * model)
{
	clipCount = model->ClipCount();
	Initialize(model->Clips().data(), model->Bones().data(), model->BoneCount());
}

ModelClipMap::ModelClipMap(ModelClip** pClips, UINT clipCount, ModelBone** pBones, UINT boneCount)
	: clipCount(clipCount)
{
	Initialize(pClips, pBones, boneCount);
}

ModelClipMap::~ModelClipMap()
{
	SafeRelease(srv);
	SafeRelease(texture);
}

void ModelClipMap::Initialize(ModelClip ** pClips, ModelBone ** pBones, UINT boneCount)
{
	if (clipCount == 0)
		return;

	UINT maxFrameCount = 0;
	for (UINT i = 0; i < clipCount; i++)
		maxFrameCount = max(maxFrameCount, pClips[i]->FrameCount());

	ClipTransform** clipTransforms = new ClipTransform*[clipCount];
	for (UINT i = 0; i < clipCount; i++)
	{
		clipTransforms[i] = new ClipTransform(maxFrameCount, boneCount);
		clipTransforms[i]->SetClipTransform(pBones, boneCount, pClips[i]);
	}

	CreateTexture(clipTransforms, boneCount, maxFrameCount);
	CreateSrv();

	for (UINT i = 0; i < clipCount; i++)
		SafeDelete(clipTransforms[i]);
	SafeDeleteArray(clipTransforms);
}

void ModelClipMap::CreateTexture(ClipTransform** clipTransforms, UINT boneCount, UINT maxFrameCount)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	// 16����Ʈ�� �ִ�� ©�� �־�ߵ� (4���� �ȼ��� ©�� ������)
	desc.Width = boneCount * 4;
	desc.Height = maxFrameCount;
	desc.ArraySize = clipCount;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;  // 16����Ʈ
	desc.Usage = D3D11_USAGE_IMMUTABLE; // �����Ұ�
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;  // srv ������ �뵵
	desc.MipLevels = 1;  // Ȯ�� ��� �� ��
	desc.SampleDesc.Count = 1; // Ȯ�� ��� �� ��

	UINT pageSize = 16 * (boneCount * 4) * maxFrameCount;

	// ����Ʈ �ʰ��ؼ� VirtualAlloc ���
	// ���� �޸�(��ũ) -> 2���� �迭 �Ҵ� (void** p = new void*[])
	void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

	//// �޸� �Ҵ� ����
	//MEMORY_BASIC_INFORMATION memory;
	//VirtualQuery(p, &memory, sizeof(MEMORY_BASIC_INFORMATION));

	// ���� �޸� �Ҵ�
	for (UINT c = 0; c < clipCount; c++)
	{
		for (UINT y = 0; y < maxFrameCount; y++)
		{
			// �̹� �ؽ���2D�� ���� �ּ�
			UINT  start = c * pageSize;

			// ������ ���� �ּ�
			void* temp = (BYTE*)p + boneCount * y * sizeof(Matrix) + start;

			// ����޸� �Ҵ�, PAGE_READWRITE : ����
			VirtualAlloc(temp, boneCount * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
			memcpy(temp, clipTransforms[c]->TransformArr2D[y], boneCount * sizeof(Matrix));
		}
	}

	D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];
	for (UINT c = 0; c < clipCount; c++)
	{
		void* temp = (BYTE*)p + c * pageSize;
		subResource[c].pSysMem = temp;
		subResource[c].SysMemPitch = boneCount * sizeof(Matrix);
		subResource[c].SysMemSlicePitch = pageSize;
	}

	Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

	SafeDeleteArray(subResource);
	VirtualFree(p, 0, MEM_RELEASE);
}

void ModelClipMap::CreateSrv()
{
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = clipCount;

	Check(D3D::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &srv));
}


// ClipTransform ///////////////////////////////////////////////////////////////////////////

inline ModelClipMap::ClipTransform::ClipTransform(UINT frameCount, UINT boneCount)
	: FrameCount(frameCount), BoneCount(boneCount)
{
	TransformArr2D = new Matrix*[FrameCount];

	for (UINT i = 0; i < FrameCount; i++)
		TransformArr2D[i] = new Matrix[BoneCount];
}

inline ModelClipMap::ClipTransform::~ClipTransform()
{
	if (FrameCount == 0)
		return;

	for (UINT i = 0; i < FrameCount; i++)
		SafeDeleteArray(TransformArr2D[i]);
	SafeDeleteArray(TransformArr2D);
}

void ModelClipMap::ClipTransform::SetClipTransform(ModelBone ** bones, UINT boneCount, ModelClip * clip)
{

	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		Matrix* boneMatrixes = new Matrix[boneCount];
		for (UINT b = 0; b < boneCount; b++)
		{
			ModelBone* bone = bones[b];
			ModelKeyframe* frame = clip->Keyframe(bone->Name());
			Matrix parent;

			int parentIndex = bone->ParentIndex();
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = boneMatrixes[parentIndex];

			if (frame == nullptr)
			{
				// �ܺο� �߰��� �� ( Attach )
				boneMatrixes[b] = parent;
				TransformArr2D[f][b] = bone->Transform() * boneMatrixes[b];
				continue;
			}

			Matrix invGlobal = bone->Transform();
			D3DXMatrixInverse(&invGlobal, nullptr, &invGlobal);

			Matrix animation;
			ModelKeyframeData& data = frame->Transforms[f];

			Matrix S, R, T;
			D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
			D3DXMatrixRotationQuaternion(&R, &data.Rotation);
			D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);

			animation = S * R * T;

			boneMatrixes[b] = animation * parent;
			TransformArr2D[f][b] = invGlobal * boneMatrixes[b];

			// T Pose
			//Matrix identity;
			//D3DXMatrixIdentity(&identity);
			//TransformArr2D[f][b] = identity;

		}// for b
		SafeDeleteArray(boneMatrixes);

	}// for f

}


// ----------------------------------------------------------------------------
// AnimationClip
// ----------------------------------------------------------------------------

ModelAnimation::ModelAnimation(ModelClip ** clips, UINT clipCount)
	: clips(clips), clipCount(clipCount)
{
}

ModelAnimation::~ModelAnimation()
{
	SafeDelete(frameBuffer);
}

void ModelAnimation::UpdateNoTweening()
{
	TweenDesc& desc = tweenDesc;
	ModelClip* clip = clips[desc.Curr.Clip];

	// ���� �ִϸ��̼�
	UpdateClipLoop(desc.Curr, clip);

	// ���� �ִϸ��̼�
	if (desc.Next.Clip > -1)
	{
		ModelClip* nextClip = clips[desc.Curr.Clip];
		NextClip();
		UpdateClipLoop(desc.Next, nextClip);
	}
}

void ModelAnimation::UpdateTweening()
{
	//KeyframeDesc& desc = keyframeDesc;
	TweenDesc& desc = tweenDesc;
	ModelClip* clip = clips[desc.Curr.Clip];

	// ���� �ִϸ��̼�
	UpdateClipLoop(desc.Curr, clip);

	// ���� �ִϸ��̼�
	if (desc.Next.Clip > -1)
	{
		ModelClip* nextClip = clips[desc.Next.Clip];

		//-> ���� ��ȯ �� ��������

		desc.RunningTime += Time::Delta();
		desc.TweenTime = desc.RunningTime * desc.TakeTimeDiv;

		if (desc.TweenTime >= 1.0f)
			NextClip();
		else
			UpdateClipLoop(desc.Next, nextClip);
	}
}

void ModelAnimation::Render()
{
	if (frameBuffer == nullptr)
		return;

	frameBuffer->Render();
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
}

void ModelAnimation::CreateBuffer(Shader * shader)
{
	SafeDelete(frameBuffer);
	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc));
	sFrameBuffer = shader->AsConstantBuffer(ShaderEffctConstantName::CB_AnimationFrame);
}


void ModelAnimation::PlayClip(UINT clip, float speed, float takeTime)
{
	tweenDesc.TakeTimeDiv = 1.0f / takeTime;
	tweenDesc.Next.Clip = clip;
	tweenDesc.Next.Speed = speed;
}

float ModelAnimation::GetClipLength(UINT clip)
{
	return static_cast<float>(clips[clip]->FrameCount());
}

float ModelAnimation::GetClipRunTime()
{
	float result = 0.0f;
	if (tweenDesc.Next.Clip == -1)
		result = tweenDesc.Curr.RunningTime;
	else
		result = tweenDesc.Next.RunningTime;
	return result;
}

void ModelAnimation::UpdateClipLoop(KeyframeDesc& desc, ModelClip* clip)
{
	float time = clip->FrameRate() * desc.Speed * Time::Delta();

	int nextSize = static_cast<int>(desc.Time);
	desc.Time -= static_cast<float>(nextSize);
	if (desc.Time < 0.0f)
	{
		// �� ���
		desc.Time += 1.0f;
		nextSize--;
	}

	if (nextSize != 0)
	{
		// desc.CurrFrame, desc.NextFrame�� unsinged int�� ���� �� ���ֱ�
		nextSize = nextSize % static_cast<int>(clip->FrameCount());
		if (nextSize < 0)
			nextSize += clip->FrameCount();

		desc.CurrFrame = (desc.CurrFrame + nextSize) % clip->FrameCount();  // Loop
		desc.NextFrame = (desc.CurrFrame + 1) % clip->FrameCount();  // Loop
	}

	desc.Time += time;
	desc.RunningTime += time;


	//// ����
	//desc.RunningTime += Time::Delta();
	//float time = 1.0f / (clip->FrameRate() * desc.Speed);
	////float time = 1.0f / clip->FrameRate() / desc.Speed;

	//if (desc.Time >= 1.0f)
	//{
	//	desc.RunningTime -= desc.Time * time;
	//	//desc.RunningTime = 0.0f;
	//	desc.CurrFrame = (desc.CurrFrame + 1) % clip->FrameCount();  // Loop
	//	desc.NextFrame = (desc.CurrFrame + 1) % clip->FrameCount();  // Loop
	//}
	//desc.Time = desc.RunningTime / time;

}

void ModelAnimation::NextClip()
{
	TweenDesc& desc = tweenDesc;
	desc.Curr = desc.Next;

	desc.Next.Clip = -1;
	desc.Next.CurrFrame = 0;
	desc.Next.NextFrame = 0;
	desc.Next.Time = 0;
	desc.Next.RunningTime = 0.0f;
	desc.RunningTime = 0.0f;
	desc.TweenTime = 0.0f;
}
