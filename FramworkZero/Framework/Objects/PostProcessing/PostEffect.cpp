#include "Framework.h"
#include "PostEffect.h"

PostEffect::PostEffect(Shader* shader)
	: shader(shader)
{

	float width = Screen::Width(), height = Screen::Height();
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	panel = new Panel(shader);
}

PostEffect::~PostEffect()
{
	//SafeRelease(shader);
}

void PostEffect::Update()
{
	panel->Update();
}

void PostEffect::BeginPreRender()
{
	viewport->RSSetViewport();
	renderTarget->PreRender(depthStencil);
}

void PostEffect::EndPreRender()
{
	RenderTarget* after = renderTarget;
	for (PostEffectEvent& action : eventEffects)
		after = action(after, depthStencil, panel);
	panel->Pass(0);
	panel->SRV(after->SRV());
}


void PostEffect::Render()
{
	panel->Render();
}

