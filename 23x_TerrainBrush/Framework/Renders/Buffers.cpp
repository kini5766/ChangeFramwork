#include "Framework.h"
#include "Buffers.h"

VertexBuffer::VertexBuffer(void * data, UINT count, UINT stride, UINT slot, bool bCpuWrite, bool bGpuWrite)
	: data(data), count(count), stride(stride), slot(slot), bCpuWrite(bCpuWrite), bGpuWrite(bGpuWrite)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = stride * count;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (bCpuWrite == false && bGpuWrite == false)
	{
		// 쓰기 불가
		desc.Usage = D3D11_USAGE_IMMUTABLE;  
	}
	else if (bCpuWrite == true && bGpuWrite == false)
	{
		// cpu 쓰기 권한
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		// Map
	}
	else if (bCpuWrite == false && bGpuWrite == true)
	{
		// gpu 쓰기 권한
		desc.Usage = D3D11_USAGE_DEFAULT;
		// update 서브리소스
	}
	else
	{
		// 테스트용
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_SUBRESOURCE_DATA subresource = { 0 };
	subresource.pSysMem = data;

	Check(D3D::GetDevice()->CreateBuffer(&desc, (data)? &subresource : nullptr, &buffer));
}

VertexBuffer::~VertexBuffer()
{
	SafeRelease(buffer);
}

void VertexBuffer::Render()
{
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(void * data, UINT count)
	: data(data), count(count)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(UINT) * count;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_IMMUTABLE;  // 쓰기 불가


	D3D11_SUBRESOURCE_DATA subresource = { 0 };
	subresource.pSysMem = data;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subresource, &buffer));
}

IndexBuffer::~IndexBuffer()
{
	SafeRelease(buffer);
}

void IndexBuffer::Render()
{
	UINT offset = 0;

	D3D::GetDC()->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, offset);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConstantBuffer::ConstantBuffer(void * data, UINT dataSize)
	: data(data), dataSize(dataSize)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = dataSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// cpu 쓰기 권한
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Check(D3D::GetDevice()->CreateBuffer(&desc, nullptr, &buffer));
}

ConstantBuffer::~ConstantBuffer()
{
	SafeRelease(buffer);
}

void ConstantBuffer::Render()
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, data, dataSize);
	}
	D3D::GetDC()->Unmap(buffer, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
