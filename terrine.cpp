#include "terrine.h"



int CTerrine::GetSize()
{
	return size;
}

bool CTerrine::IsValid()
{
	return valid;
}

int CTerrine::GetHeight(int i, int j)
{
	return height_map[i*size + j];
}

CTerrine::CTerrine()
{
	XMap = [](int i) {return i; };
	ZMap = XMap;
}


CTerrine::~CTerrine()
{
}

void CTerrine::Init(int size)
{
	this->valid = false;
	this->size = size;
	this->height_map.resize(size*size);
}

void CTerrine::Build(std::function<double(int x, int y)> func)
{
	int index = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			this->height_map[index] = func(i, j);
			index++;
		}
	}
	this->valid = true;
}




void CTerrine::BuildVoxelMesh()
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 4; j++) {
			int h = GetHeight(i, j); // map height
			float x = XMap(i);  // corner coordinate
			float z = ZMap(j);

			float x_1 = XMap(i + 1);
			float z_1 = ZMap(j + 1);
			TERRINE_TRIANGLE_VERTEX v[4] = {
				{ x,   h, z   },
				{ x,   h, z_1 },
				{ x_1, h, z   },
				{ x_1, h, z_1 }
			};

			/*TERRINE_TRIANGLE t[2] = {
				{v[0], v[1], v[2]},
				{v[2], v[1], v[3]}
			};*/

			mesh.push_back({ v[0], v[1], v[2] });
			mesh.push_back({ v[2], v[1], v[3] });


			// vertical surface
			if (j < size - 1) {
				int h_1 = GetHeight(i, j + 1);
				TERRINE_TRIANGLE_VERTEX v_h[4] = {
					{ x,   h,   z_1 },
					{ x,   h_1, z_1 },
					{ x_1, h,   z_1 },
					{ x_1, h_1, z_1 }
				};
				mesh.push_back({ v[0], v[1], v[2] });
				mesh.push_back({ v[2], v[1], v[3] });
			}
			if (i < size - 1) {
				int h_1 = GetHeight(i+1, j);
				TERRINE_TRIANGLE_VERTEX v_h[4] = {
					{ x_1, h,   z   },
					{ x_1, h,   z_1 },
					{ x_1, h_1, z   },
					{ x_1, h_1, z_1 }
				};
				mesh.push_back({ v[0], v[1], v[2] });
				mesh.push_back({ v[2], v[1], v[3] });
			}
		}
	}
}
