#include "Framework.h"
#include "SubCamera.h"

SubCamera::SubCamera(unique_ptr<Projection> projection, float width, float height)
	: PreCamera(width, height)
	, projection(move(projection))
{
	transform = new CameraTransform();
	frustum = new Frustum();
}

SubCamera::~SubCamera()
{
	SafeDelete(frustum);
	SafeDelete(transform);
}

void SubCamera::Update()
{
	Matrix V, P;
	transform->GetView(&V);
	projection->GetMatrix(&P);
	frustum->Update(V, P);
}

void SubCamera::GetView(Matrix * out)
{
	transform->GetView(out);
}

void SubCamera::GetProjection(Matrix * out)
{
	projection->GetMatrix(out);
}

void SubCamera::GetPlanes4(Plane * planes)
{
	Plane p6[6];
	frustum->Planes(p6, 6);
	planes[0] = p6[0];
	planes[1] = p6[1];
	planes[2] = p6[4];
	planes[3] = p6[5];
}
