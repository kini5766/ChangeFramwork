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

	Debug::Log->Show("");
	Debug::Log->Show("__Gizmo__");
	string str;
	Vector3 v3;

	transform->Position(&v3);
	str = "Position : " + to_string((int)v3.x) + ", " + to_string((int)v3.y) + ", " + to_string((int)v3.z);
	Debug::Log->Show(str);
	transform->RotationDegree(&v3);
	str = "Rotation : " + to_string((int)v3.x) + ", " + to_string((int)v3.y) + ", " + to_string((int)v3.z);
	Debug::Log->Show(str);
	transform->Scale(&v3);
	str = "Scale : " + to_string((int)v3.x) + ", " + to_string((int)v3.y) + ", " + to_string((int)v3.z);
	Debug::Log->Show(str);
}

void Gizmo::SetTransform(Transform * transform)
{
	this->transform = transform;
}
