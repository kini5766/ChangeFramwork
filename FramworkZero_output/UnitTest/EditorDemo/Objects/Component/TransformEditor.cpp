#include "stdafx.h"
#include "TransformEditor.h"

#include "Utilities/BinaryFile.h"

TransformEditor::TransformEditor()
{
}

TransformEditor::~TransformEditor()
{
}

void TransformEditor::RenderImGui(Transform * t)
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID("Transform");

		Vector3 pos;
		Vector3 rota1, rota2;
		Vector3 scale;
		t->Position(&pos);
		t->RotationDegree(&rota1);
		t->Scale(&scale);
		rota2 = rota1;

		ImGui::DragFloat3("Position", pos, 0.1f);
		ImGui::DragFloat3("Rotation", rota2, 0.1f);
		ImGui::DragFloat3("Scale", scale, 0.1f);

		t->Position(pos);
		t->Scale(scale);

		rota2 -= rota1;
		if (rota2.y != 0.0f)
			t->RotateYaw(rota2.y);
		if (rota2.x != 0.0f)
			t->RotatePitch(rota2.x);
		if (rota2.z != 0.0f)
			t->RotateRoll(rota2.z);

		ImGui::PopID();
	}
}

void TransformEditor::Save(Transform * t, BinaryWriter * w)
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

void TransformEditor::Load(Transform * t, BinaryReader * r)
{
	t->Position(r->Vector3());
	t->Rotation(Quaternion((float*)r->Vector4()));
	t->Scale(r->Vector3());
}
