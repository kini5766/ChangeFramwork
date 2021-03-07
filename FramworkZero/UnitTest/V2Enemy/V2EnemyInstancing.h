#pragma once

#include "V2EnemyDesc.h"

class V2EnemyInstancing
{
public:
	V2EnemyInstancing(
		const V2EnemyInput& desc,
		unique_ptr<ModelInstancing> model);
	~V2EnemyInstancing();

public:
	void Update();
	void Render();
	void PostRender();

public:
	void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints);

private:
	V2EnemyInput desc;
	unique_ptr<ModelInstancing> model;

private:
	vector<class V2EnemyInstance*> instances;
};
