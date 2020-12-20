#pragma once


class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	Color& Ambient() { return ambient; }
	Color& Specular() { return specular; }
	Vector3& Direction() { return direction; }
	Vector3& Position() { return position; }

private:
	Color ambient = Color(0, 0, 0, 1);
	Color specular = Color(0.85f, 0.85f, 0.85f, 1);
	Vector3 direction = Vector3(-0.57735026f, -0.57735026f, +0.57735026f);
	Vector3 position = Vector3(0, 0, 0);

	//class SphereCoord* rotation;
};