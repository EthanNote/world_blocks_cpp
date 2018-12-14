


//#define RUN_ENV_TEST
#ifndef RUN_ENV_TEST
#include"game.h"
#include"camera.h"


#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <memory>
#include <iostream>

#include "shader.h"
#include "block.h"
#include "renderer.h"
#include "rendertarget.h"

#include <thread>






class Test : public Game {

public:
	GLuint vbo = 0;
	GLuint vao = 0;
	GLuint vs = 0;
	GLuint fs = 0;
	GLuint shader_programme = 0;

	
	std::shared_ptr<CameraFPS> camera = std::dynamic_pointer_cast<CameraFPS, Camera>(Camera::CreateFPSCamera());

	BlockShader block_shader = nullptr;
	BlockRenderer block_renderer = nullptr;
	BlockPool block_pool = nullptr;

	ScreenShader screen_shader = nullptr;
	ScreenRenderer screen_renderer = nullptr;


	RenderTarget rendertarget = nullptr;

	virtual bool GameLoop() {
		if (!Game::GameLoop()) {
			return false;
		}

		GLuint location = glGetUniformLocation(shader_programme, "mat_mvp");

		// wipe the drawing surface clear
		glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);


		auto mvp = camera->GetMVP();
		auto mv = camera->GetModelView();
		auto projection = camera->GetProjection();

		glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);


		block_shader->mvp.Set(mvp);
		block_shader->mv.Set(mv);


		rendertarget->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		block_renderer->Draw();


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

		screen_shader->projection.Set(projection);

		screen_renderer->Draw();

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

		auto controller = camera->CreateController();
		controllers.push_back(controller);

		camera->position = glm::vec3(-1, 5, -3);

		block_shader = CBlockShader::Create();
		block_shader->Load("shader/block/block.vs", "shader/block/block.fs", "shader/block/block.gs");
		block_shader->LocateUniform("MVP", block_shader->mvp);
		block_shader->LocateUniform("MV", block_shader->mv);


		block_pool = CBlockPool::Create();
		block_renderer = CBlockRenderer::Create(block_shader, block_pool);

		rendertarget = CRenderTarget::Create();
		rendertarget->CreateDepthBuffer(800, 600);
		rendertarget->CreateColorBuffers(800, 600, 4);
		auto check = rendertarget->CheckStatus();
		if (check != RTCheckResult::COMPLETE) {
			std::cout << check << std::endl;
		}

		screen_shader = CScreenShader::Create();
		screen_shader->Load("shader/screen/screen.vs", "shader/screen/screen.fs", "shader/screen/screen.gs");
		/*screen_shader->LocateUniform("tex", screen_shader->tex);
		screen_shader->tex.Set(0);*/

		screen_shader->LocateUniform("textures", screen_shader->textures);
		screen_shader->LocateUniform("projection", screen_shader->projection);
		auto units = std::vector<TextureUnit>({ TEXTURE0,TEXTURE1 ,TEXTURE2 ,TEXTURE3 });
		screen_shader->textures.Set(units);

		screen_renderer = CScreenRenderer::Create(screen_shader);

		float points[] = {
		0.0f,  0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
		};


		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);


		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		const char* vertex_shader =
			"#version 400\n"
			"in vec3 vp;"
			"uniform mat4 mat_mvp;\n"
			"void main () {"
			"  gl_Position = mat_mvp * vec4 (vp, 1.0);"
			"}";

		const char* fragment_shader =
			"#version 400\n"
			"out vec4 frag_colour;"
			"void main () {"
			"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
			"}";

		vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader, NULL);
		glCompileShader(vs);
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader, NULL);
		glCompileShader(fs);

		shader_programme = glCreateProgram();
		glAttachShader(shader_programme, fs);
		glAttachShader(shader_programme, vs);
		glLinkProgram(shader_programme);
			   

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





#endif




#ifdef RUN_ENV_TEST

#include <GL/glew.h>

#include<GLFW/glfw3.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<stdio.h>
#include <stdlib.h>

#pragma comment(lib, "opengl32.lib")

int main(int argc, char* argv[])
{
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	float points[] = {
		0.0f,  0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"uniform mat4 mat_mvp;\n"
		"void main () {"
		"  gl_Position = mat_mvp * vec4 (vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);


	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
	glm::mat4 mvp = Projection * View;
	GLuint location = glGetUniformLocation(shader_programme, "mat_mvp");


	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_programme);
		glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// update other events like input handling 
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
#endif

