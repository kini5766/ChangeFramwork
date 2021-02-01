#include "Framework.h"
#include "LensDistortion.h"

LensDistortion::LensDistortion(Shader * shader, float width, float height)
{
	if (width == 0)
		width = Screen::Width();
	if (height == 0)
		height = Screen::Height();

	renderTarget = new RenderTarget(width, height);

	material = new ShaderSetter(shader);
	material->SetFloat("LensPower", lensPower);
	Vector3 weight = Vector3(lensDistortion, 0, 0);
	weight.y = lensDistortion + lensChromaticDistortion;
	weight.z = lensDistortion + 2 * lensChromaticDistortion;
	material->SetVector("DistortionWeight", weight);
}

LensDistortion::~LensDistortion()
{
	SafeDelete(material);
	SafeDelete(renderTarget);
}

void LensDistortion::ImGuiRender()
{
	ImGui::Separator();
	ImGui::InputFloat("LensPower", &lensPower, 0.01f);
	material->SetFloat("LensPower", lensPower);
	
	ImGui::InputFloat("Distortion", &lensDistortion, 0.001f);
	ImGui::InputFloat("ChromaticDistortion", &lensChromaticDistortion, 0.001f);
	
	Vector3 weight = Vector3(lensDistortion, 0, 0);
	weight.y = lensDistortion + lensChromaticDistortion;
	weight.z = lensDistortion + 2 * lensChromaticDistortion;
	material->SetVector("DistortionWeight", weight);
}

void LensDistortion::On(PostEffect * target)
{
	target->AddEffect(
		bind(&LensDistortion::PreRender, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

RenderTarget * LensDistortion::PreRender(RenderTarget * before, DepthStencil * depthStencil, Panel * panel)
{
	renderTarget->PreRender(depthStencil);
	{
		material->Render();
		panel->Pass(4);
		panel->SRV(before->SRV());
		panel->Render();
	}
	return renderTarget;
}
