#include "stdafx.h"
#include "ComponentCollider.h"

#include "Utilities/BinaryFile.h"

ComponentCollider::ComponentCollider()
{
}

ComponentCollider::~ComponentCollider()
{
}

void ComponentCollider::RenderImGui(ColliderBox * c)
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

		Transform* t = c->GetTransform();
		{
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
		}

		ImGui::PopID();
	}
}

void ComponentCollider::Save(ColliderBox * c, BinaryWriter * w)
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

void ComponentCollider::Load(ColliderBox * c, BinaryReader * r)
{
	Transform* t = c->GetTransform();
	t->Position(r->Vector3());
	t->Rotation(Quaternion((float*)r->Vector4()));
	t->Scale(r->Vector3());

	c->SetLayer(r->UInt());
}
