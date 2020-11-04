#pragma once

// cpu���� ���ľ��� ���� �� ����

class VertexBuffer
{
public:
	VertexBuffer(void* data, UINT count, UINT stride, UINT slot = 0, bool bCpuWrite = false, bool bGpuWrite = false);
	~VertexBuffer();

	UINT Count() { return count; }
	UINT Stride() { return stride; }
	ID3D11Buffer* Buffer() { return buffer; }

	void Render();  // ���̴��� �ڿ��� �о��ִ� ����

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

// �ð�, ����, ����ü, ��ü
// 2048 ����Ʈ ���� ����
// 16����Ʈ ����� ����(���ڶ�� �е�����(�� ���� ����) ��������)
// �� �׷��� �ٸ� ������ ���� ���� �پ� ����
// Map�� ���� ���ľ�
// �ٲ��� ������ �׳� �Ѿ

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
