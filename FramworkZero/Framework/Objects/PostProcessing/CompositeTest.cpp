#include "Framework.h"
#include "CompositeTest.h"

CompositeTest::CompositeTest(Shader * shader)
{
	float width = Screen::Width(), height = Screen::Height();
	for (UINT i = 0; i < 4; i++)
		renderTargets[i] = new RenderTarget(width, height);

	material = new ShaderSetter(shader);
	Vector2 pixelSize = Vector2(1 / Screen::Width(), 1 / Screen::Height());
	material->SetVector("PixelSize", pixelSize);

	material->SetSRV("LuminosityMap", renderTargets[0]->SRV());
	material->SetSRV("BlurXMap", renderTargets[1]->SRV());
	material->SetSRV("BlurYMap", renderTargets[2]->SRV());
}

CompositeTest::~CompositeTest()
{
	SafeDelete(material);

	for (UINT i = 0; i < 4; i++)
		SafeDelete(renderTargets[i]);
}

void CompositeTest::ImGuiRender()
{
	ImGui::SliderFloat("Threshold", &threshold, 0.1f, 0.99f);
	material->SetFloat("Threshold", threshold);

	ImGui::SliderFloat("BlurRatio", &blurRatio, 0, 1);
	material->SetFloat("BlurRatio", blurRatio);
}

RenderTarget * CompositeTest::PreRender(RenderTarget * before, DepthStencil * depthStencil, Panel * panel)
{
	material->Render();

	// luminosity
	renderTargets[0]->PreRender(depthStencil);
	{
		//panel->Pass(6);
		panel->SRV(before->SRV());
		panel->Render();
	}

	// Gaussian X
	renderTargets[1]->PreRender(depthStencil);
	{
		//panel->Pass(7);
		panel->SRV(before->SRV());
		panel->Render();
	}

	// Gaussian Y
	renderTargets[2]->PreRender(depthStencil);
	{
		//panel->Pass(8);
		panel->SRV(before->SRV());
		panel->Render();
	}

	// Composite
	renderTargets[3]->PreRender(depthStencil);
	{
		material->Render();
		//panel->Pass(5);
		panel->SRV(before->SRV());
		panel->Render();
	}

	return renderTargets[3];
}
