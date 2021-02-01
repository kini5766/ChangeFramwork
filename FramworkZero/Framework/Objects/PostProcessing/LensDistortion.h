#pragma once

class LensDistortion
{
public:
	LensDistortion(Shader* shader, float width = 0, float height = 0);
	~LensDistortion();

public:
	void ImGuiRender();

public:
	void On(PostEffect* target);
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTarget;

	float lensPower = 12.0f;
	float lensDistortion = -0.016f;
	float lensChromaticDistortion = -0.016f;
};