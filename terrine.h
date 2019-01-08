#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <shared_mutex>
#include "renderable.h"
#include "simplexnoise.h"


struct VOXEL_TERRINE_TILE {
	int x;
	int y;
	int z;
	int size;

	int grad[2];
};

struct VOXEL_TILE_AREA {
	int i;
	int j;
	int span_i;
	int span_j;
};

class CVoxelTerrine :public Renderable {
	int size = 0;
	void* GetVertexBufferPointer() override;
	int GetPrimitiveCount() override;
	CSimplexNoise* noise = new CSimplexNoise();
public:

	CVoxelTerrine();
	std::vector<VOXEL_TERRINE_TILE> tiles;
	std::vector<VOXEL_TILE_AREA> cutout_areas;
	void Init(int size);
	void Build();
	void BuildArea(VOXEL_TILE_AREA & area);
	void CutoutArea(VOXEL_TILE_AREA & area);
	void FlattenArea(VOXEL_TILE_AREA & area, int y);
};

typedef std::shared_ptr<CVoxelTerrine> TiledTerrine;


