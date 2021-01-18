#pragma once

class Saturation
{
public:
	Saturation(Shader* shader);
	~Saturation();

public:
	void ImGuiRender();

public:
	void On(PostEffect* target);
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTarget;

	float saturation = 3.0f;
};