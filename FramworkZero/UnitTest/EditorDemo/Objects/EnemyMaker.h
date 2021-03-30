#pragma once

class EnemyMakerBase
{
public:
	virtual ~EnemyMakerBase() {}
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints) = 0;
};


// --
// EnemyMakerMelee
// --
class EnemyMakerMelee : public EnemyMakerBase
{
public:
	EnemyMakerMelee(class IFocus* player);
	~EnemyMakerMelee();

public:
	// EnemyMakerBase을(를) 통해 상속됨
	void Update() override;
	void Render() override;
	void PostRender() override;
	void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints) override;

private:
	class V2EnemyMelee* enemy;
};


// --
// EnemyMakerMagic
// --
class EnemyMakerMagic : public EnemyMakerBase
{
public:
	EnemyMakerMagic(class IFocus* player);
	~EnemyMakerMagic();

public:
	// EnemyMakerBase을(를) 통해 상속됨
	void Update() override;
	void Render() override;
	void PostRender() override;
	void AddInstance(const Matrix& localWorld, const vector<Vector3>* patrolPoints) override;

private:
	class V2EnemyMagic* enemy;
};
