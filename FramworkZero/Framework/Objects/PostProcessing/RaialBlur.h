#pragma once

class RaialBlur
{
public:
	RaialBlur(Shader* shader);
	~RaialBlur();

public:
	void ImGuiRender();

public:
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTarget;

	UINT blurCount = 8;
	float radius = 0.6f;
	float amount = 0.04f;
	Vector2 center = Vector2(0.5f, 0.5f);
};