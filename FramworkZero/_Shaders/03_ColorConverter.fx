
float RGBCVtoHUE(in float3 RGB, in float C, in float V)
{
	float3 Delta = (V - RGB) / C;
	Delta.rgb -= Delta.brg;
	Delta.rgb += float3(2, 4, 6);

	Delta.brg = step(V, RGB) * Delta.brg;

	float H = max(Delta.r, max(Delta.g, Delta.b));
	return frac(H / 6);
}

float3 RGBtoHSV(in float3 RGB)
{
	float3 HSV = 0;
	HSV.z = max(RGB.r, max(RGB.g, RGB.b));
	float M = min(RGB.r, min(RGB.g, RGB.b));
	float C = HSV.z - M;
	if (C != 0)
	{
		HSV.x = RGBCVtoHUE(RGB, C, HSV.z);
		HSV.y = C / HSV.z;
	}
	return HSV;
}

float3 HUEtoRGB(in float H)
{
	float R = abs(H * 6 - 3) - 1;
	float G = 2 - abs(H * 6 - 2);
	float B = 2 - abs(H * 6 - 4);
	return saturate(float3(R, G, B));
}

float3 HSVtoRGB(float3 HSV)
{
	float3 RGB = HUEtoRGB(HSV.x) - 1;
	float3 temp = (RGB * HSV.y + 1);

	temp *= HSV.z;

	return temp;
}

float3 HSVComplement(float3 HSV)
{
	float3 complement = HSV;
	complement.x -= 0.5;
	if (complement.x < 0.0)
	{
		complement.x += 1.0;
	}

	return (complement);
}

float HueLerp(float h1, float h2, float v)
{
	float d = abs(h1 - h2);
	if (d <= 0.5)
	{
		return lerp(h1, h2, v);
	}
	else if (h1 < h2)
	{
		return frac(lerp((h1 + 1.0), h2, v));
	}
	else
	{
		return frac(lerp(h1, (h2 + 1.0), v));
	}
}
