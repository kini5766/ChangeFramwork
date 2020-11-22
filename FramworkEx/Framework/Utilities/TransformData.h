#pragma once

class TransformData
{
public:
	TransformData();
	~TransformData();

	void SetParent(TransformData* value);

private:
	void RemoveParent();
	bool RemoveChild(TransformData* value);
	void AddChild(TransformData* value);

private:
	TransformData* parent = nullptr;
	vector<TransformData*> childs;
};