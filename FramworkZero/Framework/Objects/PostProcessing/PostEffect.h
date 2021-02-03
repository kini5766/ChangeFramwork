#pragma once

typedef function<RenderTarget* (RenderTarget* before, DepthStencil* depthStencil, Panel* panel)> FuncComposite;

class PostEffect
{
public:
	PostEffect(Shader* shader, float width = 0, float height = 0);
	~PostEffect();

public:
	void Update();
	void BeginPreRender();
	void EndPreRender();
	void Render();

public:
	void ClearEffects() { funcComposites.clear(); }
	void AddEffect(const FuncComposite& value) { funcComposites.push_back(value); }
	void AddEffects(const vector<FuncComposite>& values);

	// render values
private:
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;
	Panel* panel;

	Shader* shader;
	vector<FuncComposite> funcComposites;

};