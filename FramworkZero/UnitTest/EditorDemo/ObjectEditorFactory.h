#pragma once

class ObjectEditorFactory
{
private:
	void Insert();
public:
	void InsertDebuger();

public:
	ObjectEditorFactory();
	~ObjectEditorFactory();

public:
	string* GetNames(UINT* pSize);
	class IObjectEditor* CreateEditor(string typeName);
	class IObjectDebuger* CreateDebuger(string typeName);

private:
	// ������ ���� ��
	vector<pair<string, IObjectEditor*(*)(void)>> creator;
	vector<pair<string, IObjectDebuger*(*)(void)>> debugerCreator;
	string* names;
	UINT size;
};
