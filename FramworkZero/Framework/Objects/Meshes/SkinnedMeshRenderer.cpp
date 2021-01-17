#include "Framework.h"
#include "SkinnedMeshRenderer.h"

using namespace ShaderEffectName;

SkinnedMeshRenderer::SkinnedMeshRenderer()
{
	bindPose = new MeshBindPose();
}

void SkinnedMeshRenderer::RenderInstance()
{
}

SkinnedMeshRenderer::~SkinnedMeshRenderer()
{
	for (auto d : renderers)
		SafeDelete(d);
	SafeDelete(bindPose);
}

void SkinnedMeshRenderer::Update()
{
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

void SkinnedMeshRenderer::SetMaterials(Material ** mats, UINT count)
{
	for (MeshRenderer* renderer : renderers)
	{
		renderer->Materials().clear();

		for (UINT i = 0; i < count; i++)
		{
			mats[i]->SetSRV(INV_BINDPOSE, bindPose->SrvInvBindPose);
			mats[i]->SetSRV(BONESMAP, bindPose->SrvBonesMap);
			renderer->Materials().push_back(mats[i]);
		}
	}
}

void SkinnedMeshRenderer::UpdateMaterials()
{
	for (MeshRenderer* renderer : renderers)
	{
		UINT size = renderer->Materials().size();
		for (UINT i = 0; i < size; i++)
		{
			for (Material* material : renderer->Materials())
			{
				material->SetSRV(INV_BINDPOSE, bindPose->SrvInvBindPose);
				material->SetSRV(BONESMAP, bindPose->SrvBonesMap);
			}
		}
	}
}
