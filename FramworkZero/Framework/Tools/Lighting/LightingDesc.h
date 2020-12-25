#pragma once

constexpr auto POINT_LIGHT_MAX_COUNT = 256;
constexpr auto SPOT_LIGHT_MAX_COUNT = 256;

struct DirectionalLightDesc
{
	Color Ambient = Color(0, 0, 0, 1);
	Color Specular = Color(0.85f, 0.85f, 0.85f, 1);
	Vector3 Direction = Vector3(-0.57735026f, -0.57735026f, +0.57735026f);
	float Padding;

	Vector3 Position = Vector3(0, 0, 0);
	float Padding2;
};

struct PointLightDesc
{
	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	Vector3 Position;
	float Range;

	float Intensity;
	float Padding[3];
};

struct PointLightsDesc
{
	UINT Count = 0;
	float CB_PointLights_Padding[3];

	PointLightDesc Lights[POINT_LIGHT_MAX_COUNT];
};

struct SpotLightDesc
{
	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	Vector3 Position;
	float Range;

	Vector3 Direction;
	float Angle;

	float Intensity;
	float Padding[3];
};

struct SpotLightsDesc
{
	UINT Count = 0;
	float CB_SpotLights_Padding[3];

	SpotLightDesc Lights[POINT_LIGHT_MAX_COUNT];
};
