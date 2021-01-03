#include "stdafx.h"
#include "MaterialEditor.h"

#include "Tools/ImGuiInputText.h"
#include "Utilities/BinaryFile.h"

MaterialEditor::MaterialEditor()
{
}

MaterialEditor::~MaterialEditor()
{
	SafeDelete(diffuseImGui);
	SafeDelete(specularImGui);
	SafeDelete(normalMapImGui);
}

void MaterialEditor::RenderImGui()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (diffuseImGui == nullptr)
		{
			diffuseImGui = new ImGuiInputText(64);
			diffuseImGui->Text(String::ToString(diffuseMap).c_str());
			specularImGui = new ImGuiInputText(64);
			specularImGui->Text(String::ToString(specularMap).c_str());
			normalMapImGui = new ImGuiInputText(64);
			normalMapImGui->Text(String::ToString(normalMap).c_str());
		}

		ImGui::ColorEdit4("Ambient", ambient);
		ImGui::ColorEdit4("Emissive", emissive);
		ImGui::ColorEdit4("Diffuse", diffuse);

		diffuseImGui->Render("Diffuse Map");
		if (ImGui::Button("Select Diffuse File"))
		{
			Path::OpenFileDialog(L"",
				L"Texture File\0*.PNG\0*.TGA\0*.JPG", URI::Textures,
				[&](wstring r)
			{
				diffuseImGui->Text(String::ToString(Path::GetFileName(r)).c_str());
			},
				WinDesc::GetHandle()
				);
		}

		ImGui::ColorEdit4("Specular", specular);

		specularImGui->Render("Specular Map");
		if (ImGui::Button("Select Specular File"))
		{
			Path::OpenFileDialog(L"",
				L"Texture File\0*.PNG\0*.TGA\0*.JPG", URI::Textures,
				[&](wstring r)
			{
				specularImGui->Text(String::ToString(Path::GetFileName(r)).c_str());
			},
				WinDesc::GetHandle()
				);
		}

		normalMapImGui->Render("Normal Map");
		if (ImGui::Button("Select Normal File"))
		{
			Path::OpenFileDialog(L"",
				L"Texture File\0*.PNG\0*.TGA\0*.JPG", URI::Textures,
				[&](wstring r)
			{
				normalMapImGui->Text(String::ToString(Path::GetFileName(r)).c_str());
			},
				WinDesc::GetHandle()
				);
		}

		diffuseMap = String::ToWString(diffuseImGui->Text());
		specularMap = String::ToWString(specularImGui->Text());
		normalMap = String::ToWString(normalMapImGui->Text());
	}
}

void MaterialEditor::Save(BinaryWriter * w)
{
	w->Color4f(emissive);
	w->Color4f(ambient);
	w->Color4f(diffuse);
	w->Color4f(specular);
	w->String(String::ToString(diffuseMap));
	w->String(String::ToString(specularMap));
	w->String(String::ToString(normalMap));
}

void MaterialEditor::Load(BinaryReader * r)
{
	emissive = r->Color4f();
	ambient = r->Color4f();
	diffuse = r->Color4f();
	specular = r->Color4f();
	diffuseMap = String::ToWString(r->String());
	specularMap = String::ToWString(r->String());
	normalMap = String::ToWString(r->String());

	//diffuseMap = String::ToWString(r->String());
}

void MaterialEditor::Apply(Material * mat)
{
	mat->Emissive(emissive);
	mat->Ambient(ambient);
	mat->Diffuse(diffuse);
	mat->Specular(specular);
	if (diffuseMap.size() != 0)
		mat->DiffuseMap(diffuseMap);
	if (specularMap.size() != 0)
		mat->SpecularMap(specularMap);
	if (normalMap.size() != 0)
		mat->NormalMap(normalMap);
}
