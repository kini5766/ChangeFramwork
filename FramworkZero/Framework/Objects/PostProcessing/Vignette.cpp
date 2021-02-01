#include "Framework.h"
#include "Vignette.h"

Vignette::Vignette(Shader * shader, float width, float height)
{
	if (width == 0)
		width = Screen::Width();
	if (height == 0)
		height = Screen::Height();

	renderTarget = new RenderTarget(width, height);

	material = new ShaderSetter(shader);
	material->SetFloat("VignettePower", vignettePower);
	material->SetVector("VignetteScale", Vector2(vignetteScaleX, vignetteScaleY));
}

Vignette::~Vignette()
{
	SafeDelete(material);
	SafeDelete(renderTarget);
}

void Vignette::ImGuiRender()
{
	ImGui::Separator();
	ImGui::InputFloat("VignettePower", &vignettePower, 0.05f);
	material->SetFloat("VignettePower", vignettePower);
	
	ImGui::InputFloat("VignetteScaleX", &vignetteScaleX, 0.05f);
	ImGui::InputFloat("VignetteScaleY", &vignetteScaleY, 0.05f);
	material->SetVector("VignetteScale", Vector2(vignetteScaleX, vignetteScaleY));
}

void Vignette::On(PostEffect * target)
{
	target->AddEffect(
		bind(&Vignette::PreRender, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

RenderTarget * Vignette::PreRender(RenderTarget * before, DepthStencil * depthStencil, Panel * panel)
{
	renderTarget->PreRender(depthStencil);
	{
		material->Render();
		panel->SRV(before->SRV());
		panel->Pass(3);
		panel->Render();
	}
	return renderTarget;
}
