

// --
// Terrain Values
// --

Texture2D BaseMap;
Texture2D Layer1AlphaMap;
Texture2D Layer1ColorMap;

Texture2D Layer2AlphaMap;
Texture2D Layer2ColorMap;

Texture2D Layer3AlphaMap;
Texture2D Layer3ColorMap;


// --
// function
// --

float4 GetLayerColor(float2 uv)
{
	float4 base = BaseMap.Sample(PointSampler, uv);

	float alpha1 = Layer1AlphaMap.Sample(LinearSampler, uv);
	float4 layer1 = Layer1ColorMap.Sample(LinearSampler, uv);

	float alpha2 = Layer2AlphaMap.Sample(LinearSampler, uv);
	float4 layer2 = Layer2ColorMap.Sample(LinearSampler, uv);

	float alpha3 = Layer3AlphaMap.Sample(LinearSampler, uv);
	float4 layer3 = Layer3ColorMap.Sample(LinearSampler, uv);

	//float pi = 3.1415926535;
	//alpha1 = sin(alpha1 * pi * 0.5f);
	//alpha2 = sin(alpha2 * pi * 0.5f);
	//alpha3 = sin(alpha3 * pi * 0.5f);

	float alphaBase = 0;
	alphaBase += alpha1;
	alphaBase += alpha2;
	alphaBase += alpha3;
	alphaBase = 1.0f - alphaBase;

	float4 c = 0;
	c += base * alphaBase;
	c += layer1 * alpha1;
	c += layer2 * alpha2;
	c += layer3 * alpha3;

	return c;
}