#include "Framework.h"
#include "CanvasCamera.h"

#include "Tools/Viewer/UIProjection.h"

CanvasCamera::CanvasCamera()
{
	uiProjection = new UIProjection(Screen::Width(), Screen::Height());
	projection = uiProjection;
}

CanvasCamera::~CanvasCamera()
{
	SafeDelete(uiProjection);
}

void CanvasCamera::ResizeScreen(float width, float height)
{
	uiProjection->Set(width, height);
}
