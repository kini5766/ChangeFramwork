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

void Camera::ImGuiRender()
{
	string str = string("FPS : ") + to_string(ImGui::GetIO().Framerate);
	//Debug::Log->Show(str);
	//string str = string("FPS : ") + to_string(Time::Get()->FPS());
	Gui::Get()->RenderText(5.0f, 5.0f, 1, 1, 1, str);

	Vector3 P;
	GetTransform()->Position(&P);

	EulerAngle euler = GetTransform()->RotationEuler();
	Vector3 R = euler.EulerDegree();

	str = "Camera(P) : " + to_string((int)P.x) + ", " + to_string((int)P.y) + ", " + to_string((int)P.z);
	//Gui::Get()->RenderText(5.0f, 20.0f, 1, 1, 1, str);
	Debug::Log->Show(str);

	str = "Camera(R) : " + to_string((int)R.x) + ", " + to_string((int)R.y) + ", " + to_string((int)R.z);
	//Gui::Get()->RenderText(5.0f, 35.0f, 1, 1, 1, str);
	Debug::Log->Show(str);
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
