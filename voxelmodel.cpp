#include "voxelmodel.h"



CVoxelModel::CVoxelModel()
{
}


CVoxelModel::~CVoxelModel()
{
}

void * CVoxelChunck::GetVertexBufferPointer()
{
	return &this->vbuffer[0];
}

int CVoxelChunck::GetPrimitiveCount()
{
	return vbuffer.size();
}

void CVoxelChunck::InitDense(int levelsize)
{
	int size = 1 << levelsize;
	this->size = size;
	vbuffer.resize(size*size*size, { 0,0,0,-1,0,0 });
	//memset(&vbuffer[0], 0, size*size*size * sizeof(VoxelVertex));
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

void CVoxelChunck::Init(int levelsize)
{
}

CVoxelChunck::CVoxelChunck()
{
	this->attributes.clear();
	this->attributes.push_back({ 0, 4, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), 0 });
	this->attributes.push_back({ 1, 2, GL_FLOAT, GL_FALSE, sizeof(VoxelVertex), 4 * sizeof(int) });
	this->PrimitiveSize = sizeof(VoxelVertex);
	this->PrimitiveType = GL_POINTS;
}

void CDenseVoxelChunck::Init(int levelsize)
{
	int size = 1 << levelsize;
	this->size = size;
	vbuffer.resize(size*size*size, { 0,0,0,-1,0,0 });
}

SparseVoxelChunck CDenseVoxelChunck::CreateSparse()
{
	return SparseVoxelChunck();
}

void CSparseVoxelChunck::Init(int levelsize)
{
	int size = 1 << levelsize;
	this->size = size;
	vbuffer.clear();
}

DenseVoxelChunck CSparseVoxelChunck::CreateDense()
{
	return DenseVoxelChunck();
}
