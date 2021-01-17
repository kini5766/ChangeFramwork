#include "Framework.h"
#include "RaialBlur.h"

RaialBlur::RaialBlur(Shader * shader)
{
	float width = Screen::Width(), height = Screen::Height();
	renderTarget = new RenderTarget(width, height);

	material = new ShaderSetter(shader);
	material->SetInt("RaialBlurCount", blurCount);
	material->SetFloat("RaialBlurRadius", radius);
	material->SetFloat("RaialAmount", amount);
	material->SetVector("RaialCenter", center);
}

RaialBlur::~RaialBlur()
{
	SafeDelete(material);
	SafeDelete(renderTarget);
}

void RaialBlur::ImGuiRender()
{
	ImGui::SliderInt("RaialBlurCount", (int*)&blurCount, 1, 32);
	material->SetInt("RaialBlurCount", blurCount);

	ImGui::InputFloat("RaialBlurRadius", &radius, 0.01f);
	material->SetFloat("RaialBlurRadius", radius);

	ImGui::InputFloat("RaialAmount", &amount, 0.001f);
	material->SetFloat("RaialAmount", amount);

	ImGui::SliderFloat2("RaialCenter", center, 0.0f, 1.0f);
	material->SetVector("RaialCenter", center);
}

RenderTarget * RaialBlur::PreRender(RenderTarget * before, DepthStencil * depthStencil, Panel * panel)
{
	renderTarget->PreRender(depthStencil);
	{
		material->Render();
		panel->Pass(5);
		panel->SRV(before->SRV());
		panel->Render();
	}
	return renderTarget;
}
