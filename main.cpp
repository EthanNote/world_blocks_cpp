


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

	/*GLuint block_vao = 0;
	GLuint block_vbo = 0;
	GLuint block_vs = 0;
	GLuint block_fs = 0;
	GLuint block_gs = 0;
	GLuint block_shader = 0;*/

	//std::shared_ptr<CameraFPS> camera = std::shared_ptr<CameraFPS>(new CameraFPS);
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

		glUniformMatrix4fv(location, 1, GL_FALSE, &mvp[0][0]);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 3);


		block_shader->mvp.Set(mvp);



		rendertarget->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		block_renderer->Draw();


		CRenderTarget::UnbindAll();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rendertarget->color_buffers[0]->GetName());*/
		rendertarget->color_buffers[0]->Bind(TextureUnit::TEXTURE0);

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

		camera->position = glm::vec3(1.2, 1.2, 1.2);

		block_shader = CBlockShader::Create();
		block_shader->Load("shader/block/block.vs", "shader/block/block.fs", "shader/block/block.gs");
		block_shader->LocateUniform("MVP", block_shader->mvp);


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
		screen_shader->LocateUniform("tex", screen_shader->tex);


		screen_shader->tex.Set(0);
		screen_renderer = CScreenRenderer::Create(screen_shader);



		////func();



	/*	for (int i = 0; i < 1024; i++) {
			for (int j = 0; j < 1024; j++) {
				Block b{ i,0,j,1,0,0, {-1,-1,-1,-1,-1,-1,-1,-1} };
				pool->blocks.push_back(b);
			}
		}*/




		/*pool->Alloc() = { 2,0,0,1,0,0, {-1,-1,-1,-1,-1,-1,-1,-1} };
		pool->Alloc() = { 0,0,1,1,0,0, {-1,-1,-1,-1,-1,-1,-1,-1} };
		pool->Alloc() = { 0,0,2,1,0,0, {-1,-1,-1,-1,-1,-1,-1,-1} };*/

		//renderer->UpdateBuffer();


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


		/*int blocks[] = {
			0,0,0,1
		};

		glGenBuffers(1, &block_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, block_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(blocks), blocks, GL_STATIC_DRAW);

		glGenVertexArrays(1, &block_vao);
		glBindVertexArray(block_vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, block_vbo);
		glVertexAttribPointer(0, 4, GL_INT, GL_FALSE, 16, (void*)0);*/

		/*	const char* block_vertex_shader =
				"#version 400\n"
				"layout(location=0) in ivec4 block_space;\n"
				"out ivec4 block_space_out;\n"
				"void main () {"
				"  block_space_out = block_space;"
				"}";

			const char* block_geometry_shader =
				"#version 400\n"
				"layout(points) in;\n"
				"layout(triangle_strip, max_vertices = 4) out;\n"
				"in ivec4 block_space[];\n"
				"out vec4 color;\n"
				"uniform mat4 mat_mvp;\n"
				"void main () {"
				"  vec4 pos = vec4(block_space[0].x,block_space[0].y,block_space[0].z,1.0);\n"
				"  float size = block_space[0].w;\n"
				"  gl_Position = mat_mvp*(pos + vec4(0.0,0.0,0.0,0.0));\n"
				"  color = vec4(1.0,0.0,0.0,1.0);"
				"  EmitVertex();\n"
				"  gl_Position = mat_mvp*(pos + vec4(1.0,0.0,0.0,0.0));\n"
				"  color = vec4(1.0,0.0,0.0,1.0);"
				"  EmitVertex();\n"
				"  gl_Position = mat_mvp*(pos + vec4(0.0,1.0,0.0,0.0));\n"
				"  color = vec4(1.0,0.0,0.0,1.0);"
				"  EmitVertex();\n"
				"  gl_Position = mat_mvp*(pos + vec4(1.0,1.0,0.0,0.0));\n"
				"  color = vec4(1.0,0.0,0.0,1.0);"
				"  EmitVertex();\n"
				"  EndPrimitive();\n"
				"}";

			const char* block_fragment_shader =
				"#version 400\n"
				"in vec4 color;\n"
				"void main () {\n"
				"  gl_FragColor=color;\n"
				"}";


			char err[1024];
			block_vs = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(block_vs, 1, &block_vertex_shader, NULL);
			glCompileShader(block_vs);
			GLint success;

			glGetShaderiv(block_vs, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(block_vs, 1024, NULL, err);
				printf("block_vs\n");
				printf(err);
				printf("\n");
			}

			block_fs = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(block_fs, 1, &block_fragment_shader, NULL);
			glCompileShader(block_fs);

			glGetShaderiv(block_fs, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(block_fs, 1024, NULL, err);
				printf("block_fs\n");
				printf(err);
				printf("\n");
			}

			block_gs = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(block_gs, 1, &block_geometry_shader, NULL);
			glCompileShader(block_gs);

			glGetShaderiv(block_gs, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(block_gs, 1024, NULL, err);
				printf("block_gs\n");
				printf(err);
				printf("\n");
			}

			block_shader = glCreateProgram();
			glAttachShader(block_shader, block_vs);
			glAttachShader(block_shader, block_fs);
			glAttachShader(block_shader, block_gs);
			glLinkProgram(block_shader);

			glGetProgramiv(block_shader, GL_LINK_STATUS, &success);
			if (success == 0) {
				glGetProgramInfoLog(block_shader, sizeof(err), NULL, err);
				fprintf(stderr, "Error linking shader program: '%s'\n", err);
			}


			glValidateProgram(block_shader);
			glGetProgramiv(block_shader, GL_VALIDATE_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(block_shader, sizeof(err), NULL, err);
				fprintf(stderr, "Error linking shader program: '%s'\n", err);
			}*/

		auto func = [&]() {
			for (int i = 0; i < 1024; i++) {
				for (int j = 0; j < 1024; j++) {
					Block b{ i,0,j,1,0,0, {-1,-1,-1,-1,-1,-1,-1,-1} };
					block_pool->LockWrite();
					block_pool->blocks.push_back(b);
					block_pool->UnlockWrite();
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

