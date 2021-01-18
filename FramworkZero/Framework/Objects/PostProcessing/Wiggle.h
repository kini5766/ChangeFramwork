#pragma once

class Wiggle
{
public:
	Wiggle(Shader* shader);
	~Wiggle();

public:
	void ImGuiRender();

public:
	void On(PostEffect* target);
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTarget;

	float wiggleOffsetX = 10;
	float wiggleOffsetY = 10;
	float wiggleAmountX = 0.01f;
	float wiggleAmountY = 0.01f;
};