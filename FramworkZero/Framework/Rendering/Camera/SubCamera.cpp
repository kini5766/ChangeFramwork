#include "Framework.h"
#include "SubCamera.h"


SubCamera::SubCamera(float width, float height, Projection* projection)
	: PreCamera(width, height)
	, projection(projection)
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

	GetView(&V);
	GetProjection(&P);

	frustum->Update(V, P);
}

void SubCamera::GetView(Matrix * out)
{
	transform->GetView(out);
}

void SubCamera::GetProjection(Matrix * out)
{
	if (projection == nullptr)
		Context::Get()->MainCamera()->GetProjection()->GetMatrix(out);
	else projection->GetMatrix(out);
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
