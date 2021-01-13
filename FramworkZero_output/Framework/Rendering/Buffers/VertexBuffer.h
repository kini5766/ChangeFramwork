#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, UINT count, UINT stride, UINT slot = 0, bool bCpuWirte = false, bool bGpuWrite = false);
	~VertexBuffer();

	void Render();

public:
	UINT Count() { return count; }
	UINT Stride() { return stride; }
	ID3D11Buffer* Buffer() { return buffer; }


private:
	ID3D11Buffer* buffer;

	const void* data;
	UINT count;
	UINT stride;
	UINT slot;

	bool bCpuWirte;
	bool bGpuWrite;
};