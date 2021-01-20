#pragma once

class Texture2DDesc
{
public:
	Texture2DDesc(UINT arraySize = 1);
	Texture2DDesc(Texture2DDesc * copyDesc);
	Texture2DDesc(ID3D11Texture2D * texture);
	Texture2DDesc(UINT width, UINT height, bool bCpuWirte = false, bool bGpuWrite = true, UINT arraySize = 1);
	~Texture2DDesc();

public:
	D3D11_TEXTURE2D_DESC& Desc() { return desc; }
	void SetArraySize(UINT size);
	void SetTexture(ID3D11Texture2D* value);

public:
	void SetColors(const void* data);
	ID3D11Texture2D* CreateBlankTexture();
	ID3D11Texture2D* CreateTexture(UINT stride);
	ID3D11ShaderResourceView* CreateSRV();
	ID3D11RenderTargetView* CreateRTV();

public:
	void SetColorsArray(UINT arrayIndex, const void* data);
	ID3D11Texture2D* CreateTextureArray(UINT stride);
	ID3D11ShaderResourceView* CreateSRVArray();

public:
	ID3D11Texture2D* CopyResourceTexture(ID3D11Texture2D* value);
	void MapCopyFromOutput(void* out, UINT size);
	void MapCopyToInput(const void* data, UINT size);

private:
	vector<const void*> datas;

	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* texture = nullptr;
};