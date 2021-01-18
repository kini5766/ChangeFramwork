#include "Framework.h"
#include "UIProjection.h"

UIProjection::UIProjection(float width, float height)
{
	Set(width, height, 0.0f, 0.0f, 0.0f);
}

UIProjection::~UIProjection()
{
}

void UIProjection::Set(float width, float height, float zn, float zf, float fov)
{
	Super::Set(width, height, zn, zf, fov);

	D3DXMatrixScaling(&matrix, 2.0f / width, -2.0f / height, 1.0f);
	matrix._41 -= 1.0f;
	matrix._42 += 1.0f;
}
