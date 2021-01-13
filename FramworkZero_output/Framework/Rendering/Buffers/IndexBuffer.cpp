#include "Framework.h"

IndexBuffer::IndexBuffer(void * data, UINT count)
	: data(data)
	, count(count)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(UINT) * count;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_IMMUTABLE;  // 중간에 바꿀 일이 없다.


	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = data;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, /*out*/&buffer))
}

IndexBuffer::~IndexBuffer()
{
	SafeRelease(buffer);
}

void IndexBuffer::Render()
{
	D3D::GetDC()->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}