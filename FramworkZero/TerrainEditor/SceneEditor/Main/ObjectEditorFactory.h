#pragma once

class ObjectEditorFactory
{
private:
	void Insert();

public:
	ObjectEditorFactory();
	~ObjectEditorFactory();

public:
	string* GetNames(UINT* pSize);
	class IObjectEditor* CreateEditor(string typeName);

private:
	struct Creator
	{
		string Name;
		function<IObjectEditor*()> Create;
	};
	vector<Creator> creators;
	UINT size;
	string* names;
};
