#pragma once


class TestCamera
{
public:
	TestCamera();
	virtual ~TestCamera();

public:
	virtual void Update();

public:
	void AddTarget(class IShaderCamera * value);
	void UpdateTarget(D3DXMATRIX& Projection);

	D3DXVECTOR2 Position() { return position; };


protected:
	D3DXVECTOR2 position;

private:
	D3DXMATRIX view;
	vector<class IShaderCamera> target;
};