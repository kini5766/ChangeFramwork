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
	for (MeshRenderer* renderer : renderers)
		renderer->Update();
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

void SkinnedMeshRenderer::UpdateMaterials()
{
	for (MeshRenderer* renderer : renderers)
	{
		UINT size = renderer->GetData()->SubMeshCount;
		for (UINT i = 0; i < size; i++)
		{
			for (Material* material : renderer->Materials())
			{
				material->SetSRV(CB_BonesMap, bindPose->SrvBonesMap);
			}
		}
	}
}
