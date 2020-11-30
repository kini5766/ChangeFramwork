#pragma once

// ----------------------------------------------------------------------------
// TransformData
// ----------------------------------------------------------------------------

class TransformData
{
public:
	TransformData();
	TransformData(Matrix* pMatrixGetter);
	~TransformData();


#pragma region Transform

public:
	void Set(TransformData* value);  // 복사

	void Position(float x, float y, float z);
	void Position(const Vector3& value);
	void Position(Vector3* out);

	void Scale(float x, float y, float z);
	void Scale(const Vector3& value);
	void Scale(Vector3* out);

	void RotationDegree(float x, float y, float z);
	void RotationDegree(const Vector3& value);
	void RotationDegree(Vector3* out);  // 쿼터니온에서 변환한 거라 오류 있으니 출력 용도로만 사용

	void Rotation(float x, float y, float z);
	void Rotation(const Vector3& value);

	void Rotation(const Quaternion& value);
	void Rotation(Quaternion* out);

	void RotateYawDegree(float deg);
	void RotateYaw(float rad);

	void RotatePitchDegree(float deg);
	void RotatePitch(float rad);

	void RotateRollDegree(float deg);
	void RotateRoll(float rad);

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
	void LossyWorld(const Matrix& set);
	void LossyWorld(Matrix* out);
	void UpdateWorld();
	void ReplaceMatrixGetter(Matrix* pMatrixGetter);

private:
	void ChangedWorld();

private:
	Matrix* lossyWorld;
	bool changed = false;
	bool bMyWorld;

#pragma endregion


#pragma region TransformFamily

public:
	void SetParent(TransformData* value);
	TransformData** GetChilds() { return childs.data(); }
	UINT ChildCount() { return childs.size(); }

private:
	void AddThis(TransformData* oldParent, TransformData* newparent);  // 부모에 자신 넣기
	void RemoveThis(TransformData* oldParent);  // 부모에 자신 빼기

private:
	TransformData* parent = nullptr;
	vector<TransformData*> childs;

#pragma endregion

};
