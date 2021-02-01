#pragma once

typedef function<RenderTarget* (RenderTarget* before, DepthStencil* depthStencil, Panel* panel)> PostEffectEvent;

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
	void ClearEffects() { eventEffects.clear(); }
	void AddEffect(const PostEffectEvent& value) { eventEffects.push_back(value); }

	// render values
private:
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;
	Panel* panel;

	Shader* shader;
	vector<PostEffectEvent> eventEffects;

};