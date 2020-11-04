#pragma once

namespace Xml
{
	class XMLDocument;
	class XMLElement;
}

class FileDialogXml
{
public:
	FileDialogXml();
	~FileDialogXml();

public:
	void Load(const function<void(void)>& complate = function<void(void)>());
	void Save();
	void LoadFile(const wstring& name);
	void SaveFile(const wstring& name);

	void Clear();
	void CreateDecl(const char* name);

public:
	Xml::XMLDocument* Document() { return document; }
	Xml::XMLElement* Root() { return root; }


private:
	void LoadComplated(const wstring& name);
	void SaveComplated(const wstring& name);

private:
	function<void(void)> complate;
	Xml::XMLDocument* document;
	Xml::XMLElement* root = nullptr;
};