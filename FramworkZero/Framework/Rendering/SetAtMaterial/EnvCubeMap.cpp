#include "Framework.h"
#include "EnvCubeMap.h"

#include "Rendering/Camera/PreRender/EnvCamera.h"

EnvCubeMap::EnvCubeMap(float width, float height)
{
	camera = new EnvCamera(width, height);
	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	output = new PreEnvCubeDesc();

	camera->SetProjection(width, height, 0.1f, 500.0f, 0.5f * Math::PI);
}

EnvCubeMap::~EnvCubeMap()
{
	SafeDelete(output);
	SafeDelete(buffer);
	SafeDelete(camera);
}

void EnvCubeMap::PreRender()
{
	camera->GetEnvProjection(&output->Projection);
	camera->GetEnvViews_Side6(output->Views);
	camera->PreRender();

	Context::Get()->PreRenderEnvCubeMap(output);
}

void EnvCubeMap::Render()
{
	buffer->Render();
}


Transform * EnvCubeMap::GetTransform()
{
	return camera->GetTransform();
}

void EnvCubeMap::SetProjection(float width, float height, float zNear, float zFar, float fov)
{
	camera->SetProjection(width, height, zNear, zFar, fov);
}

#include "Tools/ImGui/ImGuiSrvViewer.h"
void EnvCubeMap::ImGuiRender()
{
	//ImGuiSrvViewer::Render("srv", camera->GetSRV());
	// ¹Ý»ç, ±¼Àý, ÇÁ¸®³Ú Å×½ºÆ®
	{
		ImGui::InputInt("Type", (int*)&desc.Type);
		desc.Type %= 6;

		ImGui::SliderFloat("Alpha", &desc.Alpha, 0, 1);
		ImGui::InputFloat("RefractAmount", &desc.RefractAmount, 0.01f);

		ImGui::InputFloat("FresnelBias", &desc.FresnelBias, 0.01f);
		ImGui::InputFloat("FresnelAmount", &desc.FresnelAmount, 0.01f);
		ImGui::InputFloat("FresnelScale", &desc.FresnelScale, 0.01f);
	}
}


using namespace ShaderEffectName;
void EnvCubeMap::SetAtMaterial(ShaderSetter * target)
{
	target->SetConstantBuffer(CB_ENVCUBE, buffer->Buffer());
	target->SetSRV(ENVCUBE_MAP, camera->GetSRV());
}