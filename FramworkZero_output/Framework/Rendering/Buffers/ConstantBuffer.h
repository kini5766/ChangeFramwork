#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	void Render();  // �ȿ� �ִ� ������ ��ġ��

public:
	ID3D11Buffer* Buffer() { return buffer; }

private:
	ID3D11Buffer* buffer;

	void* data;  // ����ü
	UINT dataSize;
};