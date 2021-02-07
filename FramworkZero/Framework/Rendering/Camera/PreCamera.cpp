#include "Framework.h"
#include "PreCamera.h"

PreCamera::PreCamera(float width, float height)
{
	width = (width > 0.0f) ? width : (UINT)Screen::Width();
	height = (height > 0.0f) ? height : (UINT)Screen::Height();

	viewport = new Viewport(width, height);

	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
}

PreCamera::~PreCamera()
{
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);
	SafeDelete(viewport);
}

void PreCamera::PreRender()
{
	Context::Get()->PreRender(this);
	viewport->RSSetViewport();
	renderTarget->PreRender(depthStencil);
}

void PreCamera::GetView(Matrix * out)
{
	Context::Get()->MainCamera()->GetView(out);
}

void PreCamera::GetProjection(Matrix * out)
{
	Context::Get()->MainCamera()->GetProjection(out);
}

void PreCamera::GetPlanes4(Plane * planes)
{
	Context::Get()->MainCamera()->GetPlanes4(planes);
}