#pragma once

class Lighting
{
public:
	static Lighting* Get();

	static void Create();
	static void Delete();

private:
	static Lighting* instance;

private:
	Lighting();
	~Lighting();

public:

	Color & Ambient() { return light->Ambient(); }
	Color & Specular() { return light->Specular(); }
	Vector3 & Direction() { return light->Direction(); }
	Vector3 & Position() { return light->Position(); }

private:
	DirectionalLight* light;
};

