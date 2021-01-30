#pragma once

class EditorValue
{
public:
	EditorValue();
	~EditorValue();

public:
	void Update();
	void Render();
	void ImGuiRender();

public:
	UINT Size() const;
	bool IsSelected() const { return selectedNum != -1; }
	int GetSelected() const { return selectedNum; }

public:
	class ObjectEditor* GetObj(UINT index);
	void Clear();
	void Add(class ObjectEditor* obj);
	void Destroy(int index);

	void SelectNone();
	void Select(int index);

private:
	vector<class ObjectEditor*> objs;
	int selectedNum = -1;
};