#include "Framework.h"
#include "Context.h"

#include "Rendering/Camera/CanvasCamera.h"

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
	viewport = new Viewport(Screen::Width(), Screen::Height());

	mainCamera = unique_ptr<Camera>(new Freedom());
	canvasCamera = new CanvasCamera();
	currCamera = mainCamera.get();

	ZeroMemory(culling, sizeof(Plane) * 4);
	clipping = Plane(0, 0, 0, 0);
}

Context::~Context()
{
	SafeDelete(canvasCamera);
	SafeDelete(viewport);
}

void Context::MainCamera(unique_ptr<Camera> value)
{
	if (currCamera == mainCamera.get())
		currCamera = value.get();

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

	currCamera->GetView(&view);

	return view;
}

D3DXMATRIX Context::Projection()
{
	D3DXMATRIX projection;

	currCamera->GetProjection(&projection);

	return projection;
}

void Context::ResizeScreen()
{
	viewport->Set(Screen::Width(), Screen::Height());
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
	mainCamera->GetTransform()->Position(&P);

	EulerAngle euler = mainCamera->GetTransform()->RotationEuler();
	Vector3 R = euler.EulerDegree();

	str = "Camera(P) : " + to_string((int)P.x) + ", " + to_string((int)P.y) + ", " + to_string((int)P.z);
	//Gui::Get()->RenderText(5.0f, 20.0f, 1, 1, 1, str);
	Debug::Log->Show(str);

	str = "Camera(R) : " + to_string((int)R.x) + ", " + to_string((int)R.y) + ", " + to_string((int)R.z);
	//Gui::Get()->RenderText(5.0f, 35.0f, 1, 1, 1, str);
	Debug::Log->Show(str);
}

void Context::Render()
{
	viewport->RSSetViewport();

	currCamera = mainCamera.get();
	currCamera->GetPlanes4(culling);

	clipping = Plane(0, 0, 0, 0);
}

void Context::PreRenderMain()
{
	currCamera = mainCamera.get();
	currCamera->GetPlanes4(culling);

	clipping = Plane(0, 0, 0, 0);
}

void Context::PreRender(ICamera * preRenderCamera)
{
	currCamera = preRenderCamera;
	currCamera->GetPlanes4(culling);

	clipping = Plane(0, 0, 0, 0);
}

void Context::PostRender()
{
	currCamera = canvasCamera;
	currCamera->GetPlanes4(culling);

	clipping = Plane(0, 0, 0, 0);
}

