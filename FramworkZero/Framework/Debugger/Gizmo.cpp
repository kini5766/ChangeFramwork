#include "Framework.h"
#include "Gizmo.h"

Gizmo::Gizmo()
{
}

Gizmo::~Gizmo()
{
}

void Gizmo::Update()
{
}

void Gizmo::Render()
{
	if (transform == nullptr) return;

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();
	ImGuizmo::SetID(0);

	if (Input::Mouse()->Press(1) == false)
	{
		if (Input::Keyboard()->Press(VK_SHIFT))
		{
			if (Input::Keyboard()->Down('W'))
				operation = ImGuizmo::TRANSLATE;
			else if (Input::Keyboard()->Down('E'))
				operation = ImGuizmo::ROTATE;
			else if (Input::Keyboard()->Down('R'))
				operation = ImGuizmo::SCALE;
			else if (Input::Keyboard()->Down('Q'))
				mode = (ImGuizmo::MODE)!mode;
		}
	}

	ImGuizmo::SetRect(0, 0, Screen::Width(), Screen::Height());

	Matrix V, P;
	Context::Get()->MainCamera()->GetMatrix(&V);
	Context::Get()->GetPerspective()->GetMatrix(&P);

	Matrix W;
	transform->LossyWorld(&W);
	ImGuizmo::Manipulate(V, P, operation, mode, W);

	transform->LossyWorld(W);
}

void Gizmo::SetTransform(Transform * transform)
{
	this->transform = transform;
}
