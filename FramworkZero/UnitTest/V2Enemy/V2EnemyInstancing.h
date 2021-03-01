#pragma once

class V2EnemyInstancing
{
public:
	V2EnemyInstancing();
	~V2EnemyInstancing();

public:
	void Update();
	void Render();

public:
	ModelInstancing* model;

};
