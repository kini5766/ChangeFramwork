#include "Framework.h"
#include "Saturation.h"

Saturation::Saturation(Shader* shader)
{
	float width = Screen::Width(), height = Screen::Height();
	renderTarget = new RenderTarget(width, height);

	material = new ShaderSetter(shader);
	material->SetFloat("Saturation", saturation);
}

Saturation::~Saturation()
{
	SafeDelete(material);
	SafeDelete(renderTarget);
}

void Saturation::ImGuiRender()
{
	ImGui::SliderFloat("Saturation", &saturation, 0.01f, 5.0f);
	material->SetFloat("Saturation", saturation);
}

void Saturation::On(PostEffect * target)
{
	target->AddEffect(
		bind(&Saturation::PreRender, this, placeholders::_1, placeholders::_2, placeholders::_3)
	);
}

RenderTarget * Saturation::PreRender(RenderTarget * before, DepthStencil * depthStencil, Panel * panel)
{
	renderTarget->PreRender(depthStencil);
	{
		material->Render();
		panel->Pass(1);
		panel->SRV(before->SRV());
		panel->Render();
	}
	return renderTarget;
}
