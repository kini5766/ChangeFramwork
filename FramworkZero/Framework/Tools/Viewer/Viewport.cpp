#include "Framework.h"
#include "Viewport.h"

Viewport::Viewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	Set(width, height, x, y, minDepth, maxDepth);
}

Viewport::~Viewport()
{
	
}

void Viewport::RSSetViewport()
{
	D3D::GetDC()->RSSetViewports(1, &viewport);
}

void Viewport::Set(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	viewport.TopLeftX = this->x = x;  // 뷰포트 시작점
	viewport.TopLeftY = this->y = y;  // 뷰포트 시작점
	viewport.Width = this->width = width;  // 너비
	viewport.Height = this->height = height;  // 높이
	viewport.MinDepth = this->minDepth = minDepth;  // 1픽셀 정도 깊이는 있어야 됨
	viewport.MaxDepth = this->maxDepth = maxDepth;

	RSSetViewport();
}

Vector3 Viewport::Project(const Vector3 & source, const Matrix & W, const Matrix & V, const Matrix & P)
{
	Vector3 result;

	D3DXVec3TransformCoord(&result, &source, &(W * V * P));

	// ndc -> viewport상 화면 위치
	result.x = x + 0.5f * width * (1.0f + result.x);
	result.y = y + 0.5f * height * (1.0f - result.y);
	result.z = minDepth + (maxDepth - minDepth) * result.z;
	
	return result;
}

Vector3 Viewport::Unproject(const Vector3 & source, const Matrix & W, const Matrix & V, const Matrix & P)
{
	Vector3 result;

	result.x = (source.x - x) / width * 2.0f - 1.0f;
	result.y = (y - source.y) / height * 2.0f + 1.0f;
	result.z = (source.z - minDepth) / (maxDepth - minDepth);

	Matrix wvp;
	D3DXMatrixInverse(&wvp, nullptr, &(W * V * P));
	D3DXVec3TransformCoord(&result, &result, &wvp);

	return result;
}

void Viewport::GetMouseRay(Vector3 * outPosition, Vector3 * outDirection, Matrix * world)
{
	Matrix W;
	if (world != nullptr)
		memcpy(&W, world, sizeof(Matrix));
	else D3DXMatrixIdentity(&W);

	Viewport* vp = Context::Get()->GetViewport();

	Vector3 mouse = Input::Mouse()->GetPosition();

	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();

	mouse.z = 0.0f;
	Vector3 n = vp->Unproject(mouse, W, V, P);

	mouse.z = 1.0f;
	Vector3 f = vp->Unproject(mouse, W, V, P);

	*outPosition = n;
	*outDirection = f - n;
}