#include "Framework.h"
#include "Reflection.h"

using namespace ShaderEffectName;

Reflection::Reflection(Shader * shader, Transform* transform, float width, float height)
	: material(new ShaderSetter(shader))
	, transform(transform)
{
	reflectionCamera = new SubCamera(width, height);

}

Reflection::~Reflection()
{
	SafeDelete(reflectionCamera);
	SafeDelete(material);
}

void Reflection::Update()
{
	CameraTransform* main = Context::Get()->MainCamera()->GetTransform();
	Vector3 T, R;
	EulerAngle euler;
	main->Position(&T);
	euler = main->RotationEuler();

	Vector3 position;
	transform->Position(&position);

	R = euler.EulerRadius();
	R.x = -R.x;
	euler.SetRadius(R);

	CameraTransform* camera = reflectionCamera->GetTransform();
	camera->RotationEuler(euler);

	// * 2.0f uv 벗어남 문제 때매
	T.y = (position.y * 2.0f) - T.y;
	camera->Position(T);

	Matrix V;
	camera->GetView(&V);

	material->SetMatrix(REFLECTION_VIEW, V);
}

void Reflection::PreRender()
{
	Vector3 position;
	transform->Position(&position);
	Plane plane = Plane(0, 1, 0, -position.y + 0.0f);

	reflectionCamera->PreRender();
	Context::Get()->Clipping() = plane;
}

void Reflection::Render()
{
	material->SetSRV(REFLECTION_MAP, reflectionCamera->GetRenderTarget()->SRV());
	material->SetSRV(REFLECTION_DEPTH_MAP, reflectionCamera->GetDepthStencil()->SRV());

	material->Render();
}
