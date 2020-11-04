#pragma once

// cpu에서 고쳐쓰는 것이 더 빠름

class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT count, UINT stride, UINT slot = 0, bool bCpuWrite = false, bool bGpuWrite = false);
	~VertexBuffer();

	UINT Count() { return count; }
	UINT Stride() { return stride; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();  // 쉐이더에 자원을 밀어주는 역할

private:
	ID3D11Buffer* buffer;
	void* data;
	UINT count;
	UINT stride;
	UINT slot;
	bool bCpuWrite;
	bool bGpuWrite;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class IndexBuffer
{
public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	UINT Count() { return count; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;
	void* data;
	UINT count;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 시간, 조명, 구조체, 객체
// 2048 바이트 까지 가능
// 16바이트 배수만 가능(모자라면 패딩변수(안 쓰는 변수) 만들어야함)
// 안 그러면 다른 변수의 값을 같다 붙어 버림
// Map을 통해 고쳐씀
// 바뀌지 않으면 그냠 넘어감

class ConstantBuffer
{
public:
	ConstantBuffer(void* data, UINT dataSize);
	~ConstantBuffer();

	ID3D11Buffer* Buffer() { return buffer; }

	void Render();

private:
	ID3D11Buffer* buffer;
	void* data;
	UINT dataSize;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
