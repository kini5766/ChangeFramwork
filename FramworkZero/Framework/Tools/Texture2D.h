#pragma once

class Texture2D
{
public:
	Texture2D(UINT width, UINT height, bool bCpuWirte = false, bool bGpuWrite = false, UINT arraySize = 1);
	virtual ~Texture2D();

public:
	void SetColors(const void* data);
	void CreateTexture();
	void CreateSRV();

public:
	void SetColorsArray(const void** datas);
	void CreateTextureArray();
	void CreateArraySRV();

public:
	UINT Width() { return desc.Width; }
	void Width(UINT value) { desc.Width = value; }
	UINT Height() { return desc.Height; }
	void Height(UINT value) { desc.Height = value; }
	UINT Stride() { return stride; }
	DXGI_FORMAT Format() { return desc.Format; }
	void Format(UINT stride, DXGI_FORMAT value) { desc.Format = value; this->stride = stride; }

	ID3D11Texture2D* GetTexture() { return texture; }
	ID3D11ShaderResourceView* GetSRV() { return srv; }

protected:
	const UINT arraySize;
	UINT stride = 4u * 4u;
	D3D11_TEXTURE2D_DESC desc;

	const void** datas;

protected:
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
};