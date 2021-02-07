#pragma once

// ----------------------------------------------------------------------------
// Transform
// ----------------------------------------------------------------------------

class Transform
{
public:
	Transform();
	Transform(Matrix* pMatrixGetter);
	~Transform();


#pragma region Transform

public:
	void Position(float x, float y, float z);
	void Position(const Vector3& value);
	void Position(Vector3* out);

	void Scale(float x, float y, float z);
	void Scale(const Vector3& value);
	void Scale(Vector3* out);

	void RotationEuler(const EulerAngle& value);
	EulerAngle RotationEuler();

	void Rotation(const Quaternion& value);
	void Rotation(Quaternion* out);

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	void LocalWorld(const Matrix& set);
	void LocalWorld(Matrix* out);

private:
	Vector3 position;
	Vector3 scale;
	Quaternion rotation;

#pragma endregion


#pragma region World Matrix

public:
	void GlobalWorld(const Matrix& set);
	void GlobalWorld(Matrix* out);

public:
	void UpdateWorld();
	void ReplaceMatrixGetter(Matrix* pMatrixGetter);

private:
	void ChangedWorld();

private:
	Matrix* globalWorld;
	bool changed = false;
	bool bMyWorld;

#pragma endregion


#pragma region TransformFamily

public:
	void SetParent(Transform* value);
	void UnLink();  // 부모 자식 관계 해제
	Transform* GetParent() { return parent; }
	Transform** GetChilds() { return childs.data(); }
	UINT ChildCount() const { return childs.size(); }

private:
	void AddThis(Transform* oldParent, Transform* newparent);  // 부모에 자신 넣기
	void RemoveThis(Transform* oldParent);  // 부모에 자신 빼기

private:
	Transform* parent = nullptr;
	vector<Transform*> childs;

#pragma endregion

};
