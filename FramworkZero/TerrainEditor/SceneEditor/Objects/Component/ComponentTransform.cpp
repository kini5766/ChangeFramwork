#include "stdafx.h"
#include "ComponentTransform.h"

#include "Utilities/BinaryFile.h"

ComponentTransform::ComponentTransform()
{
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::RenderImGui(Transform * t)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID("Transform");

		Vector3 pos, scale;
		Quaternion rotation;
		t->Position(&pos);
		t->Rotation(&rotation);
		t->Scale(&scale);

		EulerAngle euler;
		euler.SetQuaternion(rotation);
		Vector3 rota1, rota2;
		rota2 = rota1 = euler.EulerDegree();

		ImGui::DragFloat3("Position", pos, 0.1f);
		ImGui::DragFloat3("Rotation", rota2, 0.1f);
		ImGui::DragFloat3("Scale", scale, 0.1f);

		euler.SetDegree(rota2 - rota1);
		euler.RotateQuaternion(&rotation);

		t->Position(pos);
		t->Rotation(rotation);
		t->Scale(scale);

		ImGui::PopID();
	}
}

void ComponentTransform::Save(Transform * t, BinaryWriter * w)
{
	Vector3 pos;
	Quaternion rota;
	Vector3 scale;

	t->Position(&pos);
	t->Rotation(&rota);
	t->Scale(&scale);

	w->Vector3(pos);
	w->Vector4((float*)rota);
	w->Vector3(scale);
}

void ComponentTransform::Load(Transform * t, BinaryReader * r)
{
	t->Position(r->Vector3());
	t->Rotation(Quaternion((float*)r->Vector4()));
	t->Scale(r->Vector3());
}
