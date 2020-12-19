#include "Framework.h"

RawBuffer::RawBuffer(void * inputData, UINT inputByte, UINT outputByte)
	: CsResource()
	, inputData(inputData), inputByte(inputByte), outputByte(outputByte)
{
	CreateBuffer();
}

RawBuffer::~RawBuffer()
{
}

void RawBuffer::CreateInput()
{
	if (inputByte == 0) return;

	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = inputByte;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;  // 용도 : srv
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;  // 로우버퍼로 사용
	desc.Usage = D3D11_USAGE_DYNAMIC;  // cpu에서 보내고 gpu에서 받기
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = inputData;

	Check(D3D::GetDevice()->CreateBuffer(&desc, (inputData != nullptr) ? &subResource : nullptr, &buffer));

	input = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateSRV()
{
	if (inputByte == 0) return;

	// 쉐이더는 4바이트가 기본

	ID3D11Buffer* buffer = (ID3D11Buffer*)input;
	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;  // TYPELESS (타입이 모른다)
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	srvDesc.BufferEx.NumElements = desc.ByteWidth / 4;  // 4Byte, R32

	Check(D3D::GetDevice()->CreateShaderResourceView(buffer, &srvDesc, &srv));
}

void RawBuffer::CreateOutput()
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	desc.ByteWidth = outputByte;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;  // 용도 : uav
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;  // 로우버퍼로 사용
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;  // cpu에서 읽기

	Check(D3D::GetDevice()->CreateBuffer(&desc, nullptr, &buffer));

	output = (ID3D11Resource*)buffer;
}

void RawBuffer::CreateUAV()
{
	ID3D11Buffer* buffer = (ID3D11Buffer*)output;
	D3D11_BUFFER_DESC desc;
	buffer->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;  // TYPELESS (타입이 모른다)
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Buffer.NumElements = desc.ByteWidth / 4;  // 4Byte, R32

	Check(D3D::GetDevice()->CreateUnorderedAccessView(buffer, &uavDesc, &uav));
}

void RawBuffer::CopyToInput(void * data)
{
	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };
	D3D::GetDC()->Map(input, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, data, inputByte);
	}
	D3D::GetDC()->Unmap(input, 0);
}

void RawBuffer::CopyFromOutput(void * data)
{
	D3D11_MAPPED_SUBRESOURCE subResource = { 0 };
	D3D::GetDC()->Map(output, 0, D3D11_MAP_READ, 0, &subResource);
	{
		memcpy(data, subResource.pData, outputByte);
	}
	D3D::GetDC()->Unmap(output, 0);
}
