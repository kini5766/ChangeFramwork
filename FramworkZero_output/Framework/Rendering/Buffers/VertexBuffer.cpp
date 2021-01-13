#include "Framework.h"

VertexBuffer::VertexBuffer(const void * data, UINT count, UINT stride, UINT slot, bool bCpuWirte, bool bGpuWrite)
	: data(data)
	, count(count)
	, stride(stride)
	, slot(slot)
	, bCpuWirte(bCpuWirte)
	, bGpuWrite(bGpuWrite)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = stride * count;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (bCpuWirte == false && bGpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE;
	}
	else if (bCpuWirte == true && bGpuWrite == false)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (bCpuWirte == false && bGpuWrite == true)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
	}
	else
	{
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = data;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, /*out*/&buffer));
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