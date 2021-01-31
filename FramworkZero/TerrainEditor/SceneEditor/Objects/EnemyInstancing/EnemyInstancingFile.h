#pragma once

class EnemyInstancingFile
{
public:
	EnemyInstancingFile(class EnemyInstancingDesc* desc);
	~EnemyInstancingFile();

public:
	void Save(class BinaryWriter * w);
	void Load(class BinaryReader * r);

private:
	class EnemyInstancingDesc* desc;
};