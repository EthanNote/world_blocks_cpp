#include"game.h"
#include"camera.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <memory>
#include <iostream>

#include "shader.h"
#include "shaderlib.h"
#include "block.h"
//#include "renderer.h"
#include "rendertarget.h"
#include "renderoperation.h"
#include "blocktree.h"
#include <thread>

#include "shell.h"
#include "perlin.h"
#include "terrine.h"
extern float ssao_kernel_256[];
extern float ssao_kernel_128[];

class Test : public Game {
public:
	std::shared_ptr<CameraFPS> camera = std::dynamic_pointer_cast<CameraFPS, Camera>(Camera::CreateFPSCamera());
	BlockPool block_pool = nullptr;
	RenderTarget rt_deferred_geometry = nullptr;
	RenderTarget rt_lightmap = nullptr;
	RenderTarget rt_ssao = nullptr;
	RenderTarget rt_merge = nullptr;

	RenderOperation screen_render_operation = CSimpleRenderOperation::Create();
	RenderOperation block_render_operation = nullptr;
	RenderOperation terrine_render_operation = nullptr;

	BlockPalette palette;
	BlockTree tree = nullptr;
	Terrine terrine = nullptr;
	virtual bool GameLoop() {
		if (!Game::GameLoop()) {
			return false;
		}

		auto mvp = camera->GetMVP();
		auto mv = camera->GetModelView();
		auto projection = camera->GetProjection();

		shaderlib::block_shader->MVP.Set(mvp);
		shaderlib::block_shader->MV.Set(mv);

		shaderlib::terrine_shader->MVP.Set(mvp);
		shaderlib::terrine_shader->MV.Set(mv);

		rt_deferred_geometry->Pass([&]() {
			shaderlib::block_shader->UseProgram();
			block_render_operation->Draw();

			if (terrine_render_operation != nullptr) {
				shaderlib::terrine_shader->UseProgram();
				terrine_render_operation->Draw();
			}
			
		});

		rt_ssao->Pass([&]() {
			shaderlib::ssao_shader->view_pos_map.Set(rt_deferred_geometry->color_buffers[2]);
			shaderlib::ssao_shader->projection.Set(projection);
			shaderlib::ssao_shader->UseProgram();
			screen_render_operation->Draw();
		});

		rt_merge->Pass([&] {
			shaderlib::merge_shader->texture_ssao.Set(rt_ssao->color_buffers[0]);
			shaderlib::merge_shader->texture_color.Set(rt_deferred_geometry->color_buffers[0]);
			shaderlib::merge_shader->UseProgram();
			screen_render_operation->Draw();

		});

		CRenderTarget::Screen()->Pass([&] {
			/*glClearColor(1, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);*/
			shaderlib::texture_shader->tex.Set(rt_merge->color_buffers[0]);
			shaderlib::texture_shader->UseProgram();
			screen_render_operation->Draw();
		});

		auto err = glGetError();

		glfwPollEvents();
		glfwSwapBuffers(window);

		return true;

	}

	virtual int Init() {
		if (Game::Init()) {
			return 1;
		}

		shaderlib::loadshaders();

		auto controller = camera->CreateController();
		controllers.push_back(controller);

		camera->position = glm::vec3(512, 48, 512);
		camera->yall = -258;
		camera->pitch = -20;
	


		block_pool = CBlockPool::Create();
		block_render_operation = CBlockRenderOperation::Create(block_pool);

		rt_deferred_geometry = CRenderTarget::Create();
		rt_deferred_geometry->CreateDepthBuffer(800, 600);
		rt_deferred_geometry->CreateColorBuffers(800, 600, 4);
		rt_deferred_geometry->CheckStatus([](RTCheckResult result) {
			if (result != RTCheckResult::COMPLETE) {
				std::cout << "rt_deferred_geometry" << result << std::endl;
			}
		});


		rt_ssao = CRenderTarget::Create();
		rt_ssao->CreateColorBuffers(800, 600, 1);
		rt_ssao->CheckStatus([](RTCheckResult result) {
			if (result != RTCheckResult::COMPLETE) {
				std::cout << "rt_ssao" << result << std::endl;
			}
		});



		rt_merge = CRenderTarget::Create();
		rt_merge->CreateColorBuffers(800, 600, 1);
		rt_merge->CreateDepthBuffer(800, 600);
		rt_merge->CheckStatus([](RTCheckResult result) {
			if (result != RTCheckResult::COMPLETE) {
				std::cout << "rt_ssao" << result << std::endl;
			}
		});


		auto units = std::vector<TextureUnit>({ TEXTURE0,TEXTURE1 ,TEXTURE2 ,TEXTURE3 });

		//shaderlib::screen_shader->textures.Set(units);
		shaderlib::ssao_shader->ssao_kernel.Set(128, ssao_kernel_128);

		this->palette.clear();

		/*this->palette.push_back({ 1,0,0,0 });
		this->palette.push_back({ 0,1,0,0 });
		this->palette.push_back({ 0,0,1,0 });
		this->palette.push_back({ 1,1,0,0 });
		for (int k = 0; k < 621; k++) {
			this->palette.push_back({ 0,0,0,0 });
		}*/

		float values[] = { 0.6, 0.25, 0.5, 0.75, 1.0 };
		for (int r = 0; r < 5; r++) {
			for (int g = 0; g < 5; g++) {
				for (int b = 0; b < 5; b++) {
					//this->palette.push_back({ values[r],values[g],values[b], 1 });
					for (int a = 0; a < 5; a++) {
						this->palette.push_back({ values[r],values[g],values[b],1 });
					}
				}
			}
		}

		shaderlib::block_shader->palette.Set(palette.size(), &palette[0].r);

		/*	tree = CBlockTree::Create(block_pool, 16);
			Block b1{ 0,0,0,0, 0,0,0, {-1,-1,-1,-1,-1,-1,-1,-1},-1 };
			tree->Insert(b1);
			Block b2{ 2,0,0,0, 1,0,0, {-1,-1,-1,-1,-1,-1,-1,-1},-1 };
			tree->Insert(b2);
			Block b3{ 4,0,0,0, 2,0,0, {-1,-1,-1,-1,-1,-1,-1,-1},-1 };
			tree->Insert(b3);
			Block b4{ 6,0,0,0, 3,0,0, {-1,-1,-1,-1,-1,-1,-1,-1},-1 };
			tree->Insert(b4);

			shell::crafting::init(tree);*/

		shell::camera::init_fps(camera);

		terrine = terrine::factory::Create();
		terrine_render_operation = CTerrineRenderOperation::Create(terrine);

		//new std::thread([&] {terrine_render_operation = CTerrineRenderOperation::Create(terrine); });

		auto func = [&]() {
			CPerlin perlin;
			perlin.persistence = 5;
			perlin.Number_Of_Octaves = 4;
			int c = 0;
			for (int i = 0; i < 64; i++) {
				block_pool->LockWrite();
				for (int j = 0; j < 64; j++) {
					double n = perlin.Noise2D(i*0.005, j*0.005);
					//printf("%d %d %lf\n", i, j, n);
					int y = int(n) + 256;
					if (y < 5)
					{
						continue;
					}
					for (int k = 0; k < 3; k++) {
						Block b{ i,y - k,j, 0,0,0, 0, {-1,-1,-1,-1,-1,-1,-1,-1}, 0 };
						block_pool->blocks.push_back(b);
					}
					c++;



					/*for (int k = 0; k <=j; k++) {
						Block b{ i,k,j,0,c%625,0, 0, {-1,-1,-1,-1,-1,-1,-1,-1}, 0 };
						block_pool->LockWrite();
						block_pool->blocks.push_back(b);
						block_pool->UnlockWrite();
						c++;
					}*/
					//std::this_thread::sleep_for(std::chrono::microseconds(0));
				}
				block_pool->UnlockWrite();
			}
		};

		new std::thread(func);

		auto error = glGetError();
		return error;
	}
};

auto game = std::shared_ptr<Test>(new Test());

int main() {

	CPerlin perlin;
	perlin.persistence = 1;
	perlin.Number_Of_Octaves = 5;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			printf("%.4lf ", perlin.Noise2D(i, j));
		}
		printf("\n");
	}

	shell::init();

	shell::start_idle();
	Game::Run(game);
	//shell::init();
	//shell::destroy();
	return 0;
}
