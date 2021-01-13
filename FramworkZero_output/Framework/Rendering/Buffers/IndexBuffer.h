#pragma once

class IndexBuffer
{
public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	void Render();

public:
	UINT Count() { return count; }
	ID3D11Buffer* Buffer() { return buffer; }

private:
	ID3D11Buffer* buffer;

	// UINT* ���ص� ��
	void* data;
	UINT count;
};