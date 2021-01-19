
// --
// filter
// --

float2 PixelSize;
float2 Offset[9] =
{
	float2(-1, -1), float2(+0, -1), float2(+1, -1),
	float2(-1, -0), float2(+0, -0), float2(+1, -0),
	float2(-1, +1), float2(+0, +1), float2(+1, +1)
};

float3 Filter(float filter[9], float2 uv)
{
	float3 sum = 0;

	[unroll(9)]
	for (uint i = 0; i < 9; i++)
	{
		uv = uv + Offset[i] * PixelSize;
		float3 c = DiffuseMap.Sample(PointSampler, uv).rgb;
		sum += c * filter[i];
	}

	return sum;
}



// --
// Sharpness
// --

float Sharpnessfilter[9] =
{
	-0.0f, -1.0f, -0.0f,
	-1.0f, +5.0f, -1.0f,
	-0.0f, -1.0f, -0.0f
};

float4 PS_Sharpness(VertexOutput input) : SV_TARGET0
{
	float3 c = Filter(Sharpnessfilter, input.Uv);
	return float4(c, 1.0f);
}

// --
// Gaussianfilter
// --

float Gaussianfilter[9] =
{
	1.0f, 2.0f, 1.0f,
	2.0f, 4.0f, 2.0f,
	1.0f, 2.0f, 1.0f
};
float4 PS_Gaussianfilter(VertexOutput input) : SV_TARGET0
{
	float3 c = Filter(Gaussianfilter, input.Uv);
	c /= 16.0f;
	return float4(c, 1.0f);
}

// --
// RaialBlur
// --

uint RaialBlurCount = 8;
float RaialBlurRadius = 0.6f;
float RaialAmount = 0.04f;
float2 RaialCenter = float2(0.5f, 0.5f);

float4 PS_RaialBlur(VertexOutput input) : SV_TARGET0
{
	float2 centerDirection = input.Uv - RaialCenter;
	float radius = length(centerDirection);
	centerDirection /= radius;

	radius = radius * 2 / RaialBlurRadius;

	//radius = saturate(radius);

	float2 uv = centerDirection * radius * radius * RaialAmount / RaialBlurCount;
	float4 sum = 0;
	for (uint i = 0; i < RaialBlurCount; i++)
	{
		sum += DiffuseMap.Sample(PointSampler, input.Uv);
		input.Uv -= uv;
	}

	sum /= RaialBlurCount;
	return float4(sum.rgb, 1);
}



// >-- 패스 등록 안 함 --< //

// --
// Interace
// --

float Stength = 1.0f;
int InteraceInterval = 2;
float4 PS_Interace(VertexOutput input) : SV_TARGET0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);

	float height = 1.0f / PixelSize.y;
	int value = (int)(floor(input.Uv.y * height + Time * 100.0f) % InteraceInterval / (InteraceInterval / 2));

	[flatten]
	if (value)
	{
		float3 w = float3(0.2126f, 0.7152f, 0.0722f);
		float y = dot(c.rgb, w);
		c.rgb = lerp(c.rgb, c.rgb * y, Stength);
	}

	return c;
}


// > -- CompositeTest -- < //

// --
// Luminosity
// --

float Threshold = 0.0f;

float4 PS_Luminosity(VertexOutput input) : SV_TARGET0
{
	float4 c = DiffuseMap.Sample(PointSampler, input.Uv);

	return saturate((c - Threshold) / (1 - Threshold));
}

// --
// Gaussian BlurX
// --

static const float Weights[13] =
{
	0.0561f, 0.133f, 0.2780f, 0.4868f, 0.7261f, 0.9231f,
	1.0f,
	0.9231f, 0.7261f, 0.4868f, 0.2780f, 0.133f, 0.0561f
};

float4 PS_GaussianBlurX(VertexOutput input) : SV_TARGET0
{
	float2 uv = input.Uv;
	float u = PixelSize.x;

	float sum = 0;
	float4 color = 0;
	for (int i = -6; i <= 6; i++)
	{
		float2 temp = uv + float2(u * (float)i, 0.0f);
		color += DiffuseMap.Sample(PointSampler, temp) * Weights[6 + i];

		sum += Weights[6 + i];
	}
	color /= sum;

	return float4(color.rgb, 1);
}


// --
// Gaussian BlurY
// --

float4 PS_GaussianBlurY(VertexOutput input) : SV_TARGET0
{
	float2 uv = input.Uv;
	float v = PixelSize.y;

	float sum = 0;
	float4 color = 0;
	for (int i = -6; i <= 6; i++)
	{
		float2 temp = uv + float2(0.0f, v * (float)i);
		color += DiffuseMap.Sample(PointSampler, temp) * Weights[6 + i];

		sum += Weights[6 + i];
	}
	color /= sum;

	return float4(color.rgb, 1);
}


// --
// CompositeTest
// --

Texture2D LuminosityMap;
Texture2D BlurXMap;
Texture2D BlurYMap;
float BlurRatio = 0.5f;

float4 PS_CompositeTest(VertexOutput input) : SV_TARGET0
{
	float4 luminosity = LuminosityMap.Sample(PointSampler, input.Uv);
	float4 blurX = BlurXMap.Sample(PointSampler, input.Uv);
	float4 blurY = BlurYMap.Sample(PointSampler, input.Uv);
	float4 diffuse = DiffuseMap.Sample(PointSampler, input.Uv);
	float4 blur = (blurX + blurY) * 0.5f;
	luminosity *= blur;
	float4 bluredColor = lerp(blur, diffuse, BlurRatio);
	return float4((luminosity + bluredColor).rgb, 1.0f);
}


// --
// Grading
// --

#include "03_ColorConverter.fx"

float Grading = 0.5f;
float Correlation = 0.5f;
float Concentration = 2.0f;

float3 PostComplement(float3 input)
{
	float3 guide = float3(1.0f, 0.5f, 0.0f);

	float3 input_hsv = RGBtoHSV(input);
	float3 hue_pole1 = RGBtoHSV(guide);
	float3 hue_pole2 = HSVComplement(hue_pole1);

	float dist1 = abs(input_hsv.x - hue_pole1.x);
	if (dist1 > 0.5)
		dist1 = 1.0 - dist1;
	float dist2 = abs(input_hsv.x - hue_pole2.x);
	if (dist2 > 0.5)
		dist2 = 1.0 - dist2;

	float descent = smoothstep(0, Correlation, input_hsv.y);


	float3 output_hsv = input_hsv;
	if (dist1 < dist2)
	{
		float c = descent * Grading * (1.0 - pow((dist1 * 2.0), 1.0 / Concentration));

		output_hsv.x = HueLerp(input_hsv.x, hue_pole1.x, c);
		output_hsv.y = lerp(input_hsv.y, hue_pole1.y, c);
	}
	else
	{
		float c = descent * Grading * (1.0 - pow((dist2 * 2.0), 1.0 / Concentration));

		output_hsv.x = HueLerp(input_hsv.x, hue_pole2.x, c);
		output_hsv.y = lerp(input_hsv.y, hue_pole2.y, c);
	}

	return HSVtoRGB(output_hsv);
}


float4 PS_Grading(VertexOutput input) : SV_Target
{
	float4 extract = DiffuseMap.Sample(LinearSampler, input.Uv);
	float3 color = PostComplement(float3(extract.r, extract.g, extract.b));

	return float4(color, 1.0f);
}