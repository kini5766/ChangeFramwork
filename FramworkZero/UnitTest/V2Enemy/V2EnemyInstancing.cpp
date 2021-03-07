#include "stdafx.h"
#include "V2EnemyInstancing.h"

#include "V2EnemyInstance.h"


V2EnemyInstancing::V2EnemyInstancing(const V2EnemyInput & desc, unique_ptr<ModelInstancing> model)
	: desc(desc), model(move(model))
{
}

V2EnemyInstancing::~V2EnemyInstancing()
{
	for (auto d : instances)
		SafeDelete(d);
	model.reset();
}

void V2EnemyInstancing::Update()
{
	for (V2EnemyInstance* i : instances)
		i->Update();

	model->Update();
}

void V2EnemyInstancing::Render()
{
	model->Render();
}

void V2EnemyInstancing::PostRender()
{
	for (V2EnemyInstance* i : instances)
		i->PostRender();
}

void V2EnemyInstancing::AddInstance(const Matrix & localWorld, const vector<Vector3>* patrolPoints)
{
	ModelInstance* i = model->AddInstance();

	V2EnemyInstanceDesc instanceDesc;
	instanceDesc.Parent = &desc;
	instanceDesc.Model = i;
	instanceDesc.localWorld = &localWorld;
	instanceDesc.PatrolPoints = patrolPoints;

	V2EnemyInstance* enemy = new V2EnemyInstance(instanceDesc);

	instances.push_back(enemy);
}
