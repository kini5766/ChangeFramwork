#include "Framework.h"
#include "Context.h"

#include "Tools/Viewer/CanvasCamera.h"

Context* Context::instance = nullptr;

Context * Context::Get()
{
	return instance;
}

void Context::Create()
{
	SafeDelete(instance);
	instance = new Context();
}

void Context::Delete()
{
	SafeDelete(instance);
}


Context::Context()
{
	mainCamera = unique_ptr<Camera>(new Freedom());
	canvasCamera = new CanvasCamera();
}

Context::~Context()
{
	SafeDelete(canvasCamera);
	mainCamera.reset();
}

void Context::MainCamera(unique_ptr<Camera> value)
{
	mainCamera.reset();
	mainCamera = move(value);
}

CanvasCamera * Context::Canvas()
{
	return canvasCamera;
}

D3DXMATRIX Context::View()
{
	Matrix view;
	mainCamera->GetMatrix(&view);

	return view;
}

D3DXMATRIX Context::Projection()
{
	D3DXMATRIX projection;
	mainCamera->GetProjection()->GetMatrix(&projection);

	return projection;
}

void Context::ResizeScreen()
{
	mainCamera->ResizeScreen(Screen::Width(), Screen::Height());
	canvasCamera->ResizeScreen(Screen::Width(), Screen::Height());
}

void Context::Update()
{
	mainCamera->Update();

	string str = string("FPS : ") + to_string(ImGui::GetIO().Framerate);
	//Debug::Log->Show(str);
	//string str = string("FPS : ") + to_string(Time::Get()->FPS());
	Gui::Get()->RenderText(5.0f, 5.0f, 1, 1, 1, str);

	//str = string("Performance : ") + to_string(Debug::Performance->Get());
	//Gui::Get()->RenderText(5.0f, 50.0f, 1, 1, 1, str);
	//Debug::Log->Show(str);

	Vector3 P;
	mainCamera->Position(&P);

	Vector3 R;
	Quaternion q;
	mainCamera->Rotation(&q);
	{
		float xy2 = 2 * q.x * q.y;
		float xz2 = 2 * q.x * q.z;
		float yz2 = 2 * q.y * q.z;

		float xx2 = 2 * q.x * q.x;
		float yy2 = 2 * q.y * q.y;
		float zz2 = 2 * q.z * q.z;

		float xw2 = 2 * q.x * q.w;
		float yw2 = 2 * q.y * q.w;
		float zw2 = 2 * q.z * q.w;

		R.x = atan2(xw2 - yz2, 1 - xx2 - zz2);
		R.y = atan2(yw2 - xz2, 1 - yy2 - zz2);
		R.z = asin(xy2 + zw2);
	}
	R *= Math::Rad2Deg;

	str = "Camera(P) : " + to_string((int)P.x) + ", " + to_string((int)P.y) + ", " + to_string((int)P.z);
	//Gui::Get()->RenderText(5.0f, 20.0f, 1, 1, 1, str);
	Debug::Log->Show(str);

	str = "Camera(R) : " + to_string((int)R.x) + ", " + to_string((int)R.y) + ", " + to_string((int)R.z);
	//Gui::Get()->RenderText(5.0f, 35.0f, 1, 1, 1, str);
	Debug::Log->Show(str);
}

void Context::Render()
{
	mainCamera->GetViewport()->RSSetViewport();
}

