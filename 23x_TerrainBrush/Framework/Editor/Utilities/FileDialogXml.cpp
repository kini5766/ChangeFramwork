#include "Framework.h"
#include "FileDialogXml.h"

#include "Utilities/Xml.h"

FileDialogXml::FileDialogXml()
{
	document = new Xml::XMLDocument();
}

FileDialogXml::~FileDialogXml()
{
	SafeDelete(document);
}

void FileDialogXml::Load(const function<void(void)>& _complate)
{
	complate = _complate;

	function<void(wstring)> f = bind(&FileDialogXml::LoadComplated, this, placeholders::_1);
	Path::OpenFileDialog(L"", L"Xml\0*.xml", L"./", f, D3D::Get()->GetDesc().Handle);
}

void FileDialogXml::Save()
{
	function<void(wstring)> f = bind(&FileDialogXml::SaveComplated, this, placeholders::_1);
	Path::SaveFileDialog(L"", L"Xml\0*.xml", L"./", f, D3D::Get()->GetDesc().Handle);
}

void FileDialogXml::Clear()
{
	SafeDelete(document);
	document = new Xml::XMLDocument();
	root = nullptr;
}

void FileDialogXml::CreateDecl(const char* name)
{
	if (nullptr != root)
	{
		// 이미 만들어짐
		assert(false);
		return;
	}

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	// <MatrixData xmlns:xsi = "http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd = "http://www.w3.org/2001/XMLSchema"/>
	root = document->NewElement(name);  // 태그
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);
}

void FileDialogXml::LoadFile(const wstring& name)
{
	Xml::XMLError error = document->LoadFile(String::ToString(name).c_str());
	assert(error == Xml::XML_SUCCESS);

	root = document->FirstChildElement();
}

void FileDialogXml::SaveFile(const wstring& name)
{
	string folder = String::ToString(Path::GetDirectoryName(name));
	string file = String::ToString(Path::GetFileName(name));

	Path::CreateFolders(folder);
	document->SaveFile((folder + file).c_str());
}


void FileDialogXml::LoadComplated(const wstring& name)
{
	LoadFile(name);
	complate();
}

void FileDialogXml::SaveComplated(const wstring& name)
{
	SaveFile(name);
}