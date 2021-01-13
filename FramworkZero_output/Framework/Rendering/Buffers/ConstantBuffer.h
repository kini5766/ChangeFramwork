#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	void Render();  // 안에 있는 데이터 고치기

public:
	ID3D11Buffer* Buffer() { return buffer; }

private:
	ID3D11Buffer* buffer;

	void* data;  // 구조체
	UINT dataSize;
};