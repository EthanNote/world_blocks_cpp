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

#include <thread>

extern float ssao_kernel_256[];
extern float ssao_kernel_128[];

class Test : public Game {
public:
	std::shared_ptr<CameraFPS> camera = std::dynamic_pointer_cast<CameraFPS, Camera>(Camera::CreateFPSCamera());
	BlockPool block_pool = nullptr;
	RenderTarget rt_deferred_geometry = nullptr;
	RenderTarget rt_lightmap = nullptr;
	RenderTarget rt_ssao = nullptr;


	RenderOperation screen_render_operation = CSimpleRenderOperation::Create();
	RenderOperation block_render_operation = nullptr;

	virtual bool GameLoop() {
		if (!Game::GameLoop()) {
			return false;
		}


		auto mvp = camera->GetMVP();
		auto mv = camera->GetModelView();
		auto projection = camera->GetProjection();

		shaderlib::block_shader->MVP.Set(mvp);
		shaderlib::block_shader->MV.Set(mv);

		rt_deferred_geometry->Pass([&]() {
			shaderlib::block_shader->UseProgram();
			block_render_operation->Draw();			
		});

		CRenderTarget::Screen()->Pass([&] {
			rt_deferred_geometry->color_buffers[0]->Bind(TextureUnit::TEXTURE0);
			rt_deferred_geometry->color_buffers[1]->Bind(TextureUnit::TEXTURE1);
			rt_deferred_geometry->color_buffers[2]->Bind(TextureUnit::TEXTURE2);
			rt_deferred_geometry->depth_buffer->Bind(TextureUnit::TEXTURE3);

			shaderlib::ssao_shader->view_pos_map.Set(TEXTURE2);
			shaderlib::ssao_shader->projection.Set(projection);
			shaderlib::ssao_shader->UseProgram();
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

		camera->position = glm::vec3(-1, 5, -3);

	
		block_pool = CBlockPool::Create();
		block_render_operation = CBlockRenderOperation::Create(block_pool);

		rt_deferred_geometry = CRenderTarget::Create();
		rt_deferred_geometry->CreateDepthBuffer(800, 600);
		rt_deferred_geometry->CreateColorBuffers(800, 600, 4);
		auto check = rt_deferred_geometry->CheckStatus();
		if (check != RTCheckResult::COMPLETE) {
			std::cout << check << std::endl;
		}

		
		auto units = std::vector<TextureUnit>({ TEXTURE0,TEXTURE1 ,TEXTURE2 ,TEXTURE3 });
		
		//shaderlib::screen_shader->textures.Set(units);
		shaderlib::ssao_shader->ssao_kernel.Set(128, ssao_kernel_128);


		auto func = [&]() {
			for (int i = 0; i < 64; i++) {
				for (int j = 0; j < 64; j++) {
					for (int k = 0; k <=j; k++) {
						Block b{ i,k,j,1,0,0, {-1,-1,-1,-1,-1,-1,-1,-1} };
						block_pool->LockWrite();
						block_pool->blocks.push_back(b);
						block_pool->UnlockWrite();
					}
					//std::this_thread::sleep_for(std::chrono::microseconds(0));
				}
			}
		};

		new std::thread(func);

		auto error = glGetError();
		return error;
	}
};

auto game = std::shared_ptr<Test>(new Test());

int main() {
	Game::Run(game);
	return 0;
}
