#include "stdafx.h"
#include "ColliderEditor.h"

#include "Utilities/BinaryFile.h"

ColliderEditor::ColliderEditor()
{
}

ColliderEditor::~ColliderEditor()
{
}

void ColliderEditor::RenderImGui(ColliderBox * c)
{
	if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushID("Collider");

		ImGui::Text(("Layer : " + to_string(c->GetLayer())).c_str());
		ImGui::InputInt("Layer", &inputMask);
		if (ImGui::Button("On"))
			c->SetLayer(c->GetLayer() | (1 << inputMask));
		ImGui::SameLine();
		if (ImGui::Button("Off"))
			c->SetLayer(c->GetLayer() & ~(1 << inputMask));
		if (ImGui::Button("Default"))
			c->SetLayer(COLLIDER_LAYER_DEFAULT);
		ImGui::SameLine();
		if (ImGui::Button("None"))
			c->SetLayer(COLLIDER_LAYER_NONE);
		ImGui::SameLine();
		if (ImGui::Button("All"))
			c->SetLayer(COLLIDER_LAYER_ALL);

		Vector3 pos;
		Vector3 rota1, rota2;
		Vector3 scale;
		Transform* t = c->GetTransform();
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

void ColliderEditor::Save(ColliderBox * c, BinaryWriter * w)
{
	Vector3 pos;
	Quaternion rota;
	Vector3 scale;
	Transform* t = c->GetTransform();

	t->Position(&pos);
	t->Rotation(&rota);
	t->Scale(&scale);

	w->Vector3(pos);
	w->Vector4((float*)rota);
	w->Vector3(scale);

	w->UInt(c->GetLayer());
}

void ColliderEditor::Load(ColliderBox * c, BinaryReader * r)
{
	Transform* t = c->GetTransform();
	t->Position(r->Vector3());
	t->Rotation(Quaternion((float*)r->Vector4()));
	t->Scale(r->Vector3());

	c->SetLayer(r->UInt());
}
