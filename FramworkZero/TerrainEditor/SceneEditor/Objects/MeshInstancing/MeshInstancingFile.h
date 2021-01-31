#pragma once

class MeshInstancingFile
{
public:
	MeshInstancingFile(class MeshInstancingDesc* desc);
	~MeshInstancingFile();

public:
	void Save(class BinaryWriter * w);
	void Load(class BinaryReader * r);

private:
	class MeshInstancingDesc* desc;
};