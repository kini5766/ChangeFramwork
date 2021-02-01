#pragma once

class CompositeTest
{
public:
	CompositeTest(Shader* shader, float width = 0, float height = 0);
	~CompositeTest();

public:
	void ImGuiRender();

public:
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTargets[4];

	float threshold = 0.5f;
	float blurRatio = 0.5f;
};