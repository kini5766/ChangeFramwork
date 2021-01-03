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
	// ������ ���� ��
	vector<pair<string, IObjectEditor*(*)(void)>> creator;
	string* names;
	UINT size;
};
