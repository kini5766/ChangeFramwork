#pragma once

class Vignette
{
public:
	Vignette(Shader* shader);
	~Vignette();

public:
	void ImGuiRender();

public:
	void On(PostEffect* target);
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTarget;

	float vignettePower = 2.0f;
	float vignetteScaleX = 0.6f;
	float vignetteScaleY = 1.0f;
};