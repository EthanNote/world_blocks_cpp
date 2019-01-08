#include "terrine.h"

#include "simplexnoise.h"

#include <thread>
#include <iostream>





void * CVoxelTerrine::GetVertexBufferPointer()
{
	return &tiles[0];
}

int CVoxelTerrine::GetPrimitiveCount()
{
	return tiles.size();
}

CVoxelTerrine::CVoxelTerrine()
{
	this->attributes.clear();
	this->attributes.push_back({ 0, 4, GL_FLOAT, GL_FALSE, sizeof(VOXEL_TERRINE_TILE), 0 });
	this->attributes.push_back({ 1, 2, GL_FLOAT, GL_FALSE, sizeof(VOXEL_TERRINE_TILE), 4 * sizeof(int) });
	this->PrimitiveSize = sizeof(VOXEL_TERRINE_TILE);
	this->PrimitiveType = GL_POINTS;
}



void CVoxelTerrine::Init(int size)
{
	tiles.resize(size*size);
	memset(&tiles[0], 0, size*size * sizeof(VOXEL_TERRINE_TILE));
	this->size = size;
}

void CVoxelTerrine::Build()
{
	VOXEL_TILE_AREA area = { 0,0,size, size };
	BuildArea(area);

}

void CVoxelTerrine::BuildArea(VOXEL_TILE_AREA & area)
{
	for (int i = area.i; i < size && i - area.i < area.span_i; i++) {
		for (int j = area.j; j < size && j - area.j < area.span_j; j++) {
			auto p = &tiles[i*size + j];
			p->x = j;
			p->z = i;
			p->size = 1;

			p->y = 32 * noise->noise(j*0.005, i*0.005); //noise (j, i);
		}
	}

	for (int i = area.i; i < size && i - area.i < area.span_i; i++) {
		for (int j = area.j; j < size - 1 && j - area.j < area.span_j; j++) {
			auto p = &tiles[i*size + j];
			p->grad[0] = (p + 1)->y - p->y;
		}
	}

	for (int i = area.i; i < size - 1 && i - area.i < area.span_i; i++) {
		for (int j = area.j; j < size && j - area.j < area.span_j; j++) {
			auto p = &tiles[i*size + j];
			p->grad[1] = (p + size)->y - p->y;
		}
	}

}

void CVoxelTerrine::CutoutArea(VOXEL_TILE_AREA & area)
{
	for (int i = area.i; i < size && i - area.i < area.span_i; i++) {
		for (int j = area.j; j < size && j - area.j < area.span_j; j++) {
			auto p = &tiles[i*size + j];
			/*	p->x = j;
				p->z = i;*/
			p->size = 0;

			//p->y = 32 * noise->noise(j*0.005, i*0.005); //noise (j, i);
		}
	}
}

void CVoxelTerrine::FlattenArea(VOXEL_TILE_AREA & area, int y)
{
	for (int i = area.i; i < size && i - area.i < area.span_i; i++) {
		for (int j = area.j; j < size && j - area.j < area.span_j; j++) {
			auto p = &tiles[i*size + j];
			p->y = y;
		}
	}

	int i = area.i - 1;
	if (i < 0) { i = 0; }
	for (; i < size && i - area.i < area.span_i; i++) {
		int j = area.j - 1;
		if (j < 0) { j = 0; }
		for (; j < size - 1 && j - area.j < area.span_j; j++) {
			auto p = &tiles[i*size + j];
			p->grad[0] = (p + 1)->y - p->y;
		}
	}

	i = area.i - 1;
	if (i < 0) { i = 0; }

	for (; i < size - 1 && i - area.i < area.span_i; i++) {
		int j = area.j - 1;
		if (j < 0) { j = 0; }
		for (; j < size && j - area.j < area.span_j; j++) {
			auto p = &tiles[i*size + j];
			p->grad[1] = (p + size)->y - p->y;
		}
	}
}
