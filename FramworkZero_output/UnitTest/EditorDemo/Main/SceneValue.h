#pragma once

class SceneValue
{
public:
	SceneValue();
	~SceneValue();

public:
	void Update();
	void Render();

public:
	void Add(class ObjectEditor* obj);
	void Destroy(UINT index);
	UINT Size() const;
	class ObjectEditor* Obj(UINT index);
	void Clear();

private:
	vector<class ObjectEditor*> objs;
};