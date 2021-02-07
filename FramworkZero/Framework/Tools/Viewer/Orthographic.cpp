#include "Framework.h"
#include "Orthographic.h"

Orthographic::Orthographic(float width, float height, float zn, float zf)
	: Projection(width, height, zn, zf, 0.0f)
{
	Set(width, height, zn, zf, 0.0f);
}

Orthographic::~Orthographic()
{
}

void Orthographic::Resize(float width, float height)
{
	Super::Resize(width, height);

	D3DXMatrixOrthoLH(&matrix, width, height, zn, zf);
}

void Orthographic::Set(float width, float height, float zn, float zf, float fov)
{
	Super::Set(width, height, zn, zf, fov);

	D3DXMatrixOrthoLH(&matrix, width, height, zn, zf);
}
