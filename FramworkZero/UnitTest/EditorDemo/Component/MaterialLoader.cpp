#include "stdafx.h"
#include "MaterialLoader.h"

#include "Tools/ImGui/ImGuiInputText.h"
#include "Utilities/BinaryFile.h"

MaterialLoader::MaterialLoader()
{
}

MaterialLoader::~MaterialLoader()
{
}

void MaterialLoader::Load(BinaryReader * r)
{
	emissive = r->Color4f();
	ambient = r->Color4f();
	diffuse = r->Color4f();
	specular = r->Color4f();
	diffuseMap = String::ToWString(r->String());
	specularMap = String::ToWString(r->String());
	normalMap = String::ToWString(r->String());
}

void MaterialLoader::Apply(Material * mat)
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
