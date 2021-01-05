#include "Framework.h"
#include "CanvasCamera.h"

CanvasCamera::CanvasCamera()
{
	D3DXMatrixLookAtLH
	(
		&view,
		&Vector3(0, 0, 0),
		&Vector3(0, 0, 1),
		&Vector3(0, 1, 0)
	);
	D3DXMatrixScaling(&projection, 2.0f / Screen::Width(), -2.0f / Screen::Height(), 1.0f);
	projection._41 -= 1.0f;
	projection._42 += 1.0f;
}

CanvasCamera::~CanvasCamera()
{
}

void CanvasCamera::ResizeScreen(float width, float height)
{
	D3DXMatrixScaling(&projection, 2.0f / width, -2.0f / height, 1.0f);
	projection._41 -= 1.0f;
	projection._42 += 1.0f;
}

D3DXMATRIX CanvasCamera::View()
{
	return view;
}

D3DXMATRIX CanvasCamera::Projection()
{
	return projection;
}
