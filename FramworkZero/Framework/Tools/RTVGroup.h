#pragma once

class RTVGroup
{
public:
	RTVGroup(float width = 0, float height = 0, UINT rtvCount = 1);
	~RTVGroup();

public:
	void RSSetViewport();
	void PreRender(UINT index = 0);

public:
	RenderTarget* GetRtv(UINT index);

private:
	DepthStencil* depthStencil;
	Viewport* viewport;
	vector<RenderTarget*> rtvs;

	float width, height;
};