#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <shared_mutex>

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

class CTerrine
{
	int size=0;
	bool valid = false;
	std::function<float(int i)> XMap;
	std::function<float(int i)> ZMap;
public:
	unsigned int vbo=0;
	int GetSize();
	bool IsValid();
	std::vector<int> height_map;
	
	std::vector<TERRINE_TRIANGLE> mesh;
	std::shared_mutex meshlock;
	
	
	int GetHeight(int i, int j);
	CTerrine();
	~CTerrine();

	void Init(int size);
	void Build(std::function<double(int x, int y)>);
	void BuildMesh();
	void BuildVoxelMesh();
};


typedef std::shared_ptr<CTerrine> Terrine;

namespace terrine {
	namespace factory {
		Terrine Create();
	}
}