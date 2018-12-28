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



#include<GL\glew.h>
void CTerrine::BuildVoxelMesh()
{
	vbo = 0;
	mesh.clear();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			meshlock.lock();

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
				mesh.push_back({ v_h[0], v_h[1], v_h[2] });
				mesh.push_back({ v_h[2], v_h[1], v_h[3] });
			}
			if (i < size - 1) {
				int h_1 = GetHeight(i + 1, j);
				TERRINE_TRIANGLE_VERTEX v_h[4] = {
					{ x_1, h,   z   },
					{ x_1, h,   z_1 },
					{ x_1, h_1, z   },
					{ x_1, h_1, z_1 }
				};
				mesh.push_back({ v_h[0], v_h[1], v_h[2] });
				mesh.push_back({ v_h[2], v_h[1], v_h[3] });
			}
			meshlock.unlock();
			std::this_thread::sleep_for(std::chrono::microseconds(0));
		}
	}
	unsigned int _vbo;
	glGenBuffers(1, &_vbo);
	if (glIsBuffer(_vbo)) {

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * 3 * 3 * sizeof(float), &mesh[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		vbo = _vbo;
		mesh.clear();
	}

}

#include "perlin.h"
#include "simplexnoise.h"

#include <thread>
#include <iostream>



Terrine terrine::factory::Create()
{
	auto t = new CTerrine;
	t->Init(1024);
	CPerlin* perlin = new CPerlin;
	perlin->persistence = 5;
	perlin->Number_Of_Octaves = 4;
	t->mesh.clear();

	CSimplexNoise* noise = new CSimplexNoise();


	/*std::cout << "Building terrine" << std::endl;
	t->Build([&perlin](int x, int y) {return perlin.Noise2D(x*0.005, y*0.005); });
	std::cout << "Generating terrine mesh" << std::endl;
	t->BuildVoxelMesh();*/

	/*new std::thread([&](CTerrine* t, CPerlin* perlin) {
		std::cout << "Building terrine" << std::endl;
		t->Build([&perlin](int x, int y) {return perlin->Noise2D(x*0.005, y*0.005); });
		std::cout << "Generating terrine mesh" << std::endl;
		t->BuildVoxelMesh();
	}, t, perlin);*/

	new std::thread([](CTerrine* t, CSimplexNoise* noise) {
		std::cout << "Building terrine" << std::endl;
		t->Build([&noise](int x, int y) {return 100*noise->noise(x*0.005, y*0.005); });
		std::cout << "Generating terrine mesh" << std::endl;
		t->BuildVoxelMesh();
	}, t, noise);

	return Terrine(t);
}

void CTiledTerrine::Init(int size)
{
	tiles.resize(size*size);
	memset(&tiles[0], 0, size*size * sizeof(TERRINE_TILE));
	this->size = size;
}

void CTiledTerrine::Build()
{
	//for (int i = 0; i < size; i++) {
	//	tiles[i].x = i;
	//	tiles[i].y = 0; // noise(i, 0);
	//	tiles[i*size].z = i;
	//	tiles[i*size].y = 0; //noise(0, i);
	//}

	for (int i = 1; i < size; i++) {
		for (int j = 0; j < size; j++) {
			auto p = &tiles[i*size + j];
			p->x = j;
			p->z = i;
			p->size = 1;

			p->y = 0; //noise (j, i);
			p->grad[0] = 0; //noise(j+i, i)
			p->grad[1] = 0; //noise(j, i+1)
		}
	}
}
