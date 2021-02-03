#include "Framework.h"
#include "PostEffect.h"

PostEffect::PostEffect(Shader* shader, float width, float height)
	: shader(shader)
{
	if (width == 0)
		width = Screen::Width();
	if (height == 0)
		height = Screen::Height();

	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	panel = new Panel(shader);

}

PostEffect::~PostEffect()
{
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
	for (FuncComposite& action : funcComposites)
		after = action(after, depthStencil, panel);
	panel->Pass(0);
	panel->SRV(after->SRV());
}


void PostEffect::Render()
{
	panel->Render();
}

void PostEffect::AddEffects(const vector<FuncComposite>& values)
{
	funcComposites.insert(funcComposites.end(), values.begin(), values.end());
}

