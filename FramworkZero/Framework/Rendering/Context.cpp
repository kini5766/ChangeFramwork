#include "Framework.h"
#include "Context.h"

#include "Rendering/Camera/CanvasCamera.h"


#pragma region ½Ì±ÛÅæ

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

#pragma endregion


#pragma region »ý¼º ¼Ò¸ê ÀÚ

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
	for (auto d : shadowCasters)
		SafeDelete(d);

	for (auto d : envCubeCasters)
		SafeDelete(d);

	SafeDelete(canvasCamera);
	SafeDelete(viewport);
}

#pragma endregion


#pragma region Ä«¸Þ¶ó

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

#pragma endregion


#pragma region À©µµ¿ì ¾÷µ¥ÀÌÆ®

void Context::ResizeScreen()
{
	viewport->Set(Screen::Width(), Screen::Height());
	mainCamera->ResizeScreen(Screen::Width(), Screen::Height());
	canvasCamera->ResizeScreen(Screen::Width(), Screen::Height());
}

void Context::Update()
{
	mainCamera->Update();

	//str = string("Performance : ") + to_string(Debug::Performance->Get());
	//Gui::Get()->RenderText(5.0f, 50.0f, 1, 1, 1, str);
	//Debug::Log->Show(str);

	mainCamera->ImGuiRender();
}

void Context::Render()
{
	viewport->RSSetViewport();

	currCamera = mainCamera.get();
	currCamera->GetPlanes_Side4(culling);

	clipping = Plane(0, 0, 0, 0);
}

void Context::PreRenderMain()
{
	currCamera = mainCamera.get();
	currCamera->GetPlanes_Side4(culling);

	clipping = Plane(0, 0, 0, 0);
}

void Context::PreRender(ICamera * preRenderCamera)
{
	currCamera = preRenderCamera;
	currCamera->GetPlanes_Side4(culling);

	clipping = Plane(0, 0, 0, 0);
}

void Context::PostRender()
{
	currCamera = canvasCamera;
	currCamera->GetPlanes_Side4(culling);

	clipping = Plane(0, 0, 0, 0);
}

#pragma endregion


#pragma region ¼Îµµ¿ì

ShadowCaster* Context::AddShadowCaster(const ShadowCasterInput & input)
{
	ShadowCaster* result = new ShadowCaster(input);
	shadowCasters.push_back(result);
	return result;
}

void Context::PreRenderShadow(PreShadowValue * input)
{
	list<ShadowCaster*>::iterator iter = shadowCasters.begin();
	while (iter != shadowCasters.end())
	{
		if ((*iter)->IsRelease())
		{
			SafeDelete(*iter);
			shadowCasters.erase(iter++);
			continue;
		}

		(*iter)->PreRender(input);

		++iter;
	}
}

#pragma endregion


#pragma region È¯°æ Å¥ºê¸Ê

EnvCubeCaster * Context::AddEnvCubeCaster(const EnvCubeCasterInput & input)
{
	EnvCubeCaster* result = new EnvCubeCaster(input);
	envCubeCasters.push_back(result);
	return result;
}

void Context::PreRenderEnvCubeMap(PreEnvCubeDesc* input)
{
	list<EnvCubeCaster*>::iterator iter = envCubeCasters.begin();
	while (iter != envCubeCasters.end())
	{
		if ((*iter)->IsRelease())
		{
			SafeDelete(*iter);
			envCubeCasters.erase(iter++);
			continue;
		}

		(*iter)->PreRender(input);

		++iter;
	}
}

#pragma endregion
