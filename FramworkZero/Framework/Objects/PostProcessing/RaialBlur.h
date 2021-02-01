#pragma once

class RaialBlur
{
public:
	RaialBlur(Shader* shader, float width = 0, float height = 0);
	~RaialBlur();

public:
	void ImGuiRender();

public:
	void On(PostEffect* target);
	RenderTarget* PreRender(RenderTarget* before, DepthStencil* depthStencil, Panel* panel);

private:
	ShaderSetter* material;
	RenderTarget* renderTarget;

	UINT blurCount = 8;
	float radius = 0.6f;
	float amount = 0.04f;
	Vector2 center = Vector2(0.5f, 0.5f);
};