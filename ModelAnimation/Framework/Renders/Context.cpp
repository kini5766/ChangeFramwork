#include "Framework.h"
#include "Context.h"
#include "Viewer/Viewport.h"
#include "Viewer/Perspective.h"
#include "Viewer/Camera.h"
#include "Viewer/Freedom.h"

Context* Context::instance = NULL;

Context * Context::Get()
{
	//assert(instance != NULL);

	return instance;
}

void Context::Create()
{
	assert(instance == NULL);

	instance = new Context();
}

void Context::Delete()
{
	SafeDelete(instance);
}

Context::Context()
{
	D3DDesc desc = D3D::GetDesc();

	// Orthographit 직교투영
	// Perfective 원근투영
	// 선생님이 만든 클래스 Perspective는 원근투영 만들어주는 클래스
	perspective = new Perspective(desc.Width, desc.Height);
	viewport = new Viewport(desc.Width, desc.Height);

	camera = new Freedom();
}

Context::~Context()
{
	SafeDelete(perspective);
	SafeDelete(viewport);
	SafeDelete(camera);
}

void Context::ResizeScreen()
{
	perspective->Set(D3D::Width(), D3D::Height());
	viewport->Set(D3D::Width(), D3D::Height());
}

void Context::Update()
{
	camera->Update();
}

void Context::Render()
{
	viewport->RSSetViewport();

	string str = string("FPS : " + to_string(ImGui::GetIO().Framerate));
	Gui::Get()->RenderText(5, 5, 1, 1, 1, str);

	Vector3 camPos;
	camera->Position(&camPos);

	str = "camera(P) : ";
	str += to_string((int)camPos.x);
	str += ", " + to_string((int)camPos.y);
	str += ", " + to_string((int)camPos.z);
	Gui::Get()->RenderText(5, 20, 1, 1, 1, str);

	Vector3 camDir;
	camera->RotationDegree(&camDir);

	str = "camera(R) : ";
	str += to_string((int)camDir.x);
	str += ", " + to_string((int)camDir.y);
	str += ", " + to_string((int)camDir.z);
	Gui::Get()->RenderText(5, 35, 1, 1, 1, str);
}

D3DXMATRIX Context::View()
{
	Matrix view;
	camera->GetMatrix(&view);
	return view;
}

D3DXMATRIX Context::Projection()
{
	D3DXMATRIX projection;
	perspective->GetMatrix(&projection);

	return projection;
}
