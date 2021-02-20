#include "Framework.h"
#include "ShadowCamera.h"

ShadowCamera::ShadowCamera(const Vector3 & at, float radius, float width, float height)
	: at(at), radius(radius), width(width), height(height)
{
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);
	viewport = new Viewport(width, height);
	frustum = new Frustum();
}

ShadowCamera::~ShadowCamera()
{
	SafeDelete(frustum);
	SafeDelete(viewport);
	SafeDelete(depthStencil);
	SafeDelete(renderTarget);
}

void ShadowCamera::GetView(Matrix * out)
{
	memcpy(out, &view, sizeof(Matrix));
}

void ShadowCamera::GetProjection(Matrix * out)
{
	memcpy(out, &projection, sizeof(Matrix));
}

void ShadowCamera::GetPlanes_Side4(Plane * planes)
{
	Plane p6[6];
	frustum->Planes(p6, 6);
	planes[0] = p6[0];
	planes[1] = p6[1];
	planes[2] = p6[4];
	planes[3] = p6[5];
}


void ShadowCamera::Update()
{
	Vector3 up = Vector3(0, 1, 0);
	Vector3 direction = Lighting::Get()->DirectionalDesc()->Direction;
	Vector3 eye = direction * radius * -2.0f;
	eye += at;
	D3DXMatrixLookAtLH(&view, &eye, &at, &up);

	Vector3 center;
	D3DXVec3TransformCoord(&center, &at, &view);
	float l = center.x - radius;
	float r = center.x + radius;
	float t = center.y + radius;
	float b = center.y - radius;
	float f = center.z + radius;
	float n = center.z - radius;
	D3DXMatrixOrthoLH(&projection, r - l, t - b, n, f);

	frustum->Update(view, projection);
}

void ShadowCamera::PreRender()
{
	Context::Get()->PreRenderMain();
	viewport->RSSetViewport();
	renderTarget->PreRender(depthStencil);
}
