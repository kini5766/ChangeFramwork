#include "Framework.h"
#include "Wiggle.h"

Wiggle::Wiggle(Shader * shader)
{
	float width = Screen::Width(), height = Screen::Height();
	renderTarget = new RenderTarget(width, height);

	material = new ShaderSetter(shader);
	material->SetVector("WiggleOffset", Vector2(wiggleOffsetX, wiggleOffsetY));
	material->SetVector("WiggleAmount", Vector2(wiggleAmountX, wiggleAmountY));
}

Wiggle::~Wiggle()
{
	SafeDelete(material);
	SafeDelete(renderTarget);
}

void Wiggle::ImGuiRender()
{
	ImGui::InputFloat("OffsetX", &wiggleOffsetX, 0.1f);
	ImGui::Separator();
	ImGui::InputFloat("OffsetY", &wiggleOffsetY, 0.1f);
	material->SetVector("WiggleOffset", Vector2(wiggleOffsetX, wiggleOffsetY));
	
	ImGui::InputFloat("AmountX", &wiggleAmountX, 0.001f);
	ImGui::Separator();
	ImGui::InputFloat("AmountY", &wiggleAmountY, 0.001f);
	material->SetVector("WiggleAmount", Vector2(wiggleAmountX, wiggleAmountY));
}

void Wiggle::On(PostEffect * target)
{
	target->AddEffect(
		bind(&Wiggle::PreRender, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);

}

RenderTarget * Wiggle::PreRender(RenderTarget * before, DepthStencil * depthStencil, Panel * panel)
{
	renderTarget->PreRender(depthStencil);
	{
		material->Render();
		panel->Pass(2);
		panel->SRV(before->SRV());
		panel->Render();
	}
	return renderTarget;
}
