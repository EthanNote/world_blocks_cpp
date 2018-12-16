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
	RenderTarget rendertarget = nullptr;

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


		rendertarget->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderlib::block_shader->UseProgram();
		block_render_operation->Draw();


		CRenderTarget::UnbindAll();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);

		// layout(location=0) out vec4 color_out;
		// layout(location=1) out vec4 normal_out;
		// layout(location=2) out vec4 position_out;
		rendertarget->color_buffers[0]->Bind(TextureUnit::TEXTURE0);
		rendertarget->color_buffers[1]->Bind(TextureUnit::TEXTURE1);
		rendertarget->color_buffers[2]->Bind(TextureUnit::TEXTURE2);
		rendertarget->depth_buffer->Bind(TextureUnit::TEXTURE3);

		shaderlib::screen_shader->projection.Set(projection);
		shaderlib::screen_shader->UseProgram();
		screen_render_operation->Draw();

		//glUseProgram(0);
		

		auto err = glGetError();

		// update other events like input handling 
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
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

		rendertarget = CRenderTarget::Create();
		rendertarget->CreateDepthBuffer(800, 600);
		rendertarget->CreateColorBuffers(800, 600, 4);
		auto check = rendertarget->CheckStatus();
		if (check != RTCheckResult::COMPLETE) {
			std::cout << check << std::endl;
		}

		
		auto units = std::vector<TextureUnit>({ TEXTURE0,TEXTURE1 ,TEXTURE2 ,TEXTURE3 });
		
		shaderlib::screen_shader->textures.Set(units);
		shaderlib::screen_shader->ssao_kernel.Set(128, ssao_kernel_128);


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
