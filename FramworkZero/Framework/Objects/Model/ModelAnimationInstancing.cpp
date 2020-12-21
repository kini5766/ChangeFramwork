#include "Framework.h"
#include "ModelAnimationInstancing.h"

using namespace ShaderEffectName;

ModelAnimationInstancing::ModelAnimationInstancing(ModelData * data)
{
	// clipCount
	boneCount = data->BoneCount();

	// Todo : Ŭ���� �����

	// keyframeCount[clipIndex * boneCount + boneIndex]
	keyframeCount = new KeyframeCount[clipCount * boneCount];
	for (UINT c = 0; c < clipCount; c++)
	{
		for (UINT b = 0; b < boneCount; b++)
		{
			KeyframeCount& k = keyframeCount[c * boneCount + b];
			k.Translation;
			k.Rotaion;
			k.Scale;
		}
	}

	CreateCompute();
}

ModelAnimationInstancing::~ModelAnimationInstancing()
{
	SafeDeleteArray(keyframeCount);

	SafeDelete(computeOutputBuffer);
	SafeDelete(computeCountBuffer);
	SafeDelete(blendBuffer);
	SafeDelete(computeShader);
}

ID3D11ShaderResourceView * ModelAnimationInstancing::GetOutputSrv()
{
	return computeOutputBuffer->OutputSRV();
}

void ModelAnimationInstancing::Update()
{
	computeShader->Render();
	computeShader->GetShader()->Dispatch(0, 0, boneCount, 1, 1);
}

void ModelAnimationInstancing::CreateCompute()
{
	computeShader = new ShaderSetter(L"02_AnimationInstance.fxo");

	// in : �������� * �ν��Ͻ�
	blendBuffer = new ConstantBuffer(keyframeDesc, sizeof(KeyframeDesc) * MODEL_INSTANCE_MAX_COUNT);
	computeShader->SetConstantBuffer(CB_Keyframes, blendBuffer->Buffer());

	// in : Ŭ��Ʈ������
	computeShader->SetSRV("InputClipMap", srvClipBoneMap);

	Texture2D t2d = Texture2D(boneCount, clipCount);
	t2d.Format(4u * 4u, DXGI_FORMAT_R32G32B32A32_UINT);
	t2d.SetColors(keyframeCount);
	t2d.CreateTexture();

	// in : Ű������ �� �ִ�ġ * (�� ���� * Ŭ�� ����)
	computeCountBuffer = new TextureBuffer(t2d.GetTexture());
	computeShader->SetSRV("InputKeyframeCount", computeCountBuffer->SRV());

	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = boneCount * 4u;
	desc.Height = MODEL_INSTANCE_MAX_COUNT;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &texture));

	// out : ���� ��*�ν��Ͻ� texture
	computeOutputBuffer = new TextureBuffer(texture);
	computeShader->SetUAV("Output", computeOutputBuffer->UAV());
	SafeRelease(texture);  // �ؽ��� ����Ǽ� ������ �ʿ� ����
}
