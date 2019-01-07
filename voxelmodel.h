#pragma once


#include"renderable.h"
#include<list>
#include<memory>
struct VoxelVertex {
	int x;
	int y;
	int z;
	int level;
	int color;
};

class CVoxelChunck: Renderable {
	std::vector<VoxelVertex> vbuffer;
	int size = 0;
	void* GetVertexBufferPointer() override;
	int GetPrimitiveCount() override;
public:
	VoxelVertex frame;
	void InitDense(int levelsize);
	void InitSparse(int levelsize, int count);
	bool OccoludeTest();
};

typedef std::shared_ptr<CVoxelChunck> VoxelChunck;

class CVoxelModel
{
public:

	std::list<VoxelChunck> chuncks;

	CVoxelModel();
	~CVoxelModel();
};

