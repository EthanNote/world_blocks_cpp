#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <shared_mutex>
#include "renderable.h"
#include "simplexnoise.h"

struct TERRINE_TRIANGLE_VERTEX {
	float x;
	float y;
	float z;
};

struct TERRINE_TRIANGLE {
	TERRINE_TRIANGLE_VERTEX v1;
	TERRINE_TRIANGLE_VERTEX v2;
	TERRINE_TRIANGLE_VERTEX v3;
};

struct TERRINE_TILE {
	int x;
	int y;
	int z;
	int size;

	int grad[2];
};

struct TILE_AREA {
	int i;
	int j;
	int span_i;
	int span_j;
};

class CTiledTerrine :public Renderable {
	int size = 0;
	void* GetVertexBufferPointer() override;
	int GetPrimitiveCount() override;
	CSimplexNoise* noise = new CSimplexNoise();
public:

	CTiledTerrine();
	std::vector<TERRINE_TILE> tiles;
	std::vector<TILE_AREA> cutout_areas;
	void Init(int size);
	void Build();
	void BuildArea(TILE_AREA & area);
	void CutoutArea(TILE_AREA & area);
	void FlattenArea(TILE_AREA & area, int y);
};

typedef std::shared_ptr<CTiledTerrine> TiledTerrine;


