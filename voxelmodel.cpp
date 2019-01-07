#include "voxelmodel.h"



CVoxelModel::CVoxelModel()
{
}


CVoxelModel::~CVoxelModel()
{
}

void CVoxelChunck::InitDense(int levelsize)
{
	int size = 1 << levelsize;
	this->size = size;
	vbuffer.clear();
	vbuffer.reserve(size*size*size);
}

void CVoxelChunck::InitSparse(int levelsize, int count)
{
	int size = 1 << levelsize;
	this->size = size;
	vbuffer.clear();
	vbuffer.reserve(count);
}

bool CVoxelChunck::OccoludeTest()
{
	return false;
}
