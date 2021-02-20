#include "Framework.h"
#include "SkinnedMeshRenderer.h"

using namespace ShaderEffectName;

SkinnedMeshRenderer::SkinnedMeshRenderer(SkinnedMeshData* desc)
{
	// Set InvBindPose
	{
		UINT boneCount = desc->MatrixBones.size();
		Matrix* boneDesc = new Matrix[boneCount * 2];

		for (UINT i = 0; i < boneCount; i++)
		{
			D3DXMatrixInverse(&boneDesc[i * 2 + 0], nullptr, desc->MatrixBones[i]);
			memcpy(&boneDesc[i * 2 + 1], desc->MatrixBones[i], sizeof(Matrix));
		}

		Texture2DDesc tex2DDesc;
		D3D11_TEXTURE2D_DESC& desc = tex2DDesc.Desc();
		desc.Width = boneCount * 2 * 4;
		desc.Height = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;  // 쉐이더에 사용할 텍스쳐
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		tex2DDesc.SetColors(boneDesc);

		invBindPose = tex2DDesc.CreateTexture(4u * 4u);
		srvInvBindPose = tex2DDesc.CreateSRV();
		SafeDeleteArray(boneDesc);
	}

	UINT materialCount = desc->Materials.size();
	for (UINT i = 0; i < materialCount; i++)
	{
		desc->Materials[i]->SetSRV(INV_BINDPOSE, srvInvBindPose);
		//desc.Materials[i]->SetSRV(BONESMAP, bindPose.SrvBonesMap);
	}

	UINT meshCount = desc->Meshes.size();
	renderers.reserve(meshCount);
	ShaderSetter** materials = (ShaderSetter**)desc->Materials.data();
	for (UINT i = 0; i < meshCount; i++)
	{
		renderers.push_back(new MeshRenderer(desc->Meshes[i], materials, materialCount));
	}
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	for (auto d : renderers)
		SafeDelete(d);

	SafeRelease(srvInvBindPose);
	SafeRelease(invBindPose);
}

void SkinnedMeshRenderer::Render()
{
	for (MeshRenderer* renderer : renderers)
		renderer->Render();
}

void SkinnedMeshRenderer::RenderInstance(UINT drawCount)
{
	for (MeshRenderer* renderer : renderers)
		renderer->RenderInstance(drawCount);
}

void SkinnedMeshRenderer::Pass(UINT value)
{
	for (MeshRenderer* renderer : renderers)
		renderer->Pass(value);
}
