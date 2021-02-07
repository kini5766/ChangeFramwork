#include "Framework.h"
#include "CanvasCamera.h"

#include "Tools/Viewer/UIProjection.h"

CanvasCamera::CanvasCamera()
	: Camera(unique_ptr<Projection>(new UIProjection(Screen::Width(), Screen::Height())))
{
}

CanvasCamera::~CanvasCamera()
{
}
