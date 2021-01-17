#pragma once

class LensDistortion
{
public:
	LensDistortion(Shader* shader);
	~LensDistortion();

public:
	void ImGuiRender();

public:
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTarget;

	float lensPower = 12.0f;
	float lensDistortion = -0.016f;
	float lensChromaticDistortion = -0.016f;
};