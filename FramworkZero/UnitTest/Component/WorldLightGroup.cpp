#include "stdafx.h"
#include "WorldLightGroup.h"

WorldLightGroup::WorldLightGroup()
{
	//Lighting::Get()->GetBurntLight()->GetDesc()->Scale = Vector3(16, 15, 14);
	//Lighting::Get()->GetBurntLight()->GetDesc()->Position = Vector3(12, 6, 0);
	//Lighting::Get()->GetBurntLight()->ApplyLight();

	PointLightDesc point;
	point =
	{
	   Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
	   Color(0.0f, 0.3f, 1.0f, 1.0f), //Diffuse
	   Color(0.0f, 0.0f, 0.7f, 1.0f), //Specular
	   Color(0.0f, 0.0f, 0.7f, 1.0f), //Emissive
	   Vector3(15, 3, 0), 8.0f, 0.9f
	};
	Lighting::Get()->AddPointLight(point);

	SpotLightDesc spot;
	spot =
	{
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Color(0.7f, 1.0f, 0.0f, 1.0f),
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Color(0.3f, 1.0f, 0.0f, 1.0f),
	   Vector3(-15, 20, -30), 25.0f,
	   Vector3(0, -1, 0), 30.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(spot);

	spot =
	{
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Color(1.0f, 0.2f, 0.9f, 1.0f),
	   Vector3(0, 20, -30), 30.0f,
	   Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Lighting::Get()->AddSpotLight(spot);
}

WorldLightGroup::~WorldLightGroup()
{
}

void WorldLightGroup::Render()
{
	// imgui
	{
		if (ImGui::Button("unSelect"))
		{
			Debug::Gizmo->SetTransform(nullptr);
		}
		if (ImGui::Button("Direction Light"))
		{
			Debug::Gizmo->SetTransform(Lighting::Get()->GetDirectional()->GetTransform());
		}
		if (ImGui::Button("Point Light"))
		{
			Debug::Gizmo->SetTransform(Lighting::Get()->GetPointLight(0)->GetTransform());
		}
		if (ImGui::Button("Burnt Light"))
		{
			Debug::Gizmo->SetTransform(Lighting::Get()->GetBurntLight()->GetTransform());
		}
		if (ImGui::Button("Spot Light"))
		{
			Debug::Gizmo->SetTransform(Lighting::Get()->GetSpotLight(0)->GetTransform());
		}
	}

	Lighting::Get()->GetDirectional()->ApplyTransform();
	Lighting::Get()->GetPointLight(0)->ApplyTransform();
	Lighting::Get()->GetBurntLight()->ApplyTransform();
	Lighting::Get()->GetSpotLight(0)->ApplyTransform();
}
