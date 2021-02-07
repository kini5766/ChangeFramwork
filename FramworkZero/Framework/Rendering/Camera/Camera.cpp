#include "Framework.h"
#include "Camera.h"

Camera::Camera(unique_ptr<Projection> projection)
	: projection(move(projection))
{
	transform = new CameraTransform();
	frustum = new Frustum();
}

Camera::~Camera()
{
	SafeDelete(frustum);
	SafeDelete(transform);
}

void Camera::Update()
{
	Matrix V, P;
	transform->GetView(&V);
	projection->GetMatrix(&P);
	frustum->Update(V, P);
}

void Camera::ResizeScreen(float width, float height)
{
	projection->Resize(width, height);
}

void Camera::GetView(Matrix * out)
{
	transform->GetView(out);
}

void Camera::GetProjection(Matrix * out)
{
	projection->GetMatrix(out);
}

void Camera::GetPlanes4(Plane * planes)
{
	Plane p6[6];
	frustum->Planes(p6, 6);
	planes[0] = p6[0];
	planes[1] = p6[1];
	planes[2] = p6[4];
	planes[3] = p6[5];
}
