#pragma once

typedef function<RenderTarget* (RenderTarget* before, DepthStencil* depthStencil, Panel* panel)> PostEffectEvent;

class PostEffect
{
public:
	PostEffect(Shader* shader);
	~PostEffect();

public:
	void Update();
	void BeginPreRender();
	void EndPreRender();
	void Render();

public:
	vector<PostEffectEvent>& EffectEvents() { return eventEffects; }

	// render values
private:
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;
	Panel* panel;

	Shader* shader;
	vector<PostEffectEvent> eventEffects;

};