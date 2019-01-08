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
	int facemask;
	int type_id;
};


class CVoxelChunck: Renderable {
protected:
	std::vector<VoxelVertex> vbuffer;
	int size = 0;
	void* GetVertexBufferPointer() override;
	int GetPrimitiveCount() override;
public:
	VoxelVertex frame;
	void InitDense(int levelsize);
	void InitSparse(int levelsize, int count);
	bool OccoludeTest();
	virtual void Init(int levelsize);
	CVoxelChunck();
};

typedef std::shared_ptr<CVoxelChunck> VoxelChunck;

class CDenseVoxelChunck;
class CSparseVoxelChunck;

typedef std::shared_ptr<CDenseVoxelChunck> DenseVoxelChunck;
typedef std::shared_ptr<CSparseVoxelChunck> SparseVoxelChunck;

class CDenseVoxelChunck : CVoxelChunck {
public:
	void Init(int levelsize) override;
	SparseVoxelChunck CreateSparse();
};

class CSparseVoxelChunck : CVoxelChunck {
public:
	void Init(int levelsize) override;
	DenseVoxelChunck CreateDense();
};


class CVoxelModel
{
public:

	std::list<VoxelChunck> chuncks;

	CVoxelModel();
	~CVoxelModel();
};

