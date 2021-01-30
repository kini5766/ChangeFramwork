#pragma once

class MeshInstancingFile
{
public:
	MeshInstancingFile(struct MeshInstancingDesc* desc);
	~MeshInstancingFile();

public:
	void Save(class BinaryWriter * w);
	void Load(class BinaryReader * r);

private:
	struct MeshInstancingDesc* desc;
};