#include "Framework.h"
#include "RTVGroup.h"

RTVGroup::RTVGroup(float width, float height, UINT rtvCount)
{
	if (width <= 0.0f)
		width = Screen::Width();
	if (height <= 0.0f)
		height = Screen::Height();

	this->width = width;
	this->width = height;

	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);
	rtvs.assign(rtvCount, new RenderTarget(width, height));
}

RTVGroup::~RTVGroup()
{
	for (auto d : rtvs)
		SafeDelete(d);
	SafeDelete(viewport);
	SafeDelete(depthStencil);
}

void RTVGroup::RSSetViewport()
{
	viewport->RSSetViewport();
}

void RTVGroup::PreRender(UINT index)
{
	rtvs[index]->PreRender(depthStencil);
}

RenderTarget * RTVGroup::GetRtv(UINT index)
{
	return rtvs[index];
}
