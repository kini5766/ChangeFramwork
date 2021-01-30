#pragma once

class ObjectEditorList
{
public:
	ObjectEditorList();
	~ObjectEditorList();

public:
	UINT Size() const;
	bool IsSelected() const { return selectedNum != -1; }
	int GetSelected() const { return selectedNum; }

public:
	class ObjectEditor* GetObj(UINT index);
	void Clear();
	ObjectEditor* CreateEditor(struct EditorDesc* desc);
	void Destroy(int index);

	void SelectNone();
	void Select(int index);

private:
	vector<class ObjectEditor*> objs;
	int selectedNum = -1;
};