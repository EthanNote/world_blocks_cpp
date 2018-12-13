#include "game.h"

#pragma comment(lib, "opengl32.lib")

std::shared_ptr<Game> current=nullptr;

extern void drag_init(GLFWwindow* window);
extern void drag_update(GLFWwindow* window);
//std::shared_ptr<Game> Game::GetCurrent()
//{
//	//return current;
//	//return nullptr;
//}

int Game::Init()
{
	/*if (current != nullptr) {
		fprintf(stderr, "ERROR: Game instance exists\n");
		return 1;
	}
	current = std::shared_ptr<Game>(this);*/

	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
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



	drag_init(window);
	return 0;
}

int Game::Uninit()
{
	glfwTerminate();
	return 0;
}




int Game::Run(std::shared_ptr<Game> game)
{
	if (current != nullptr) {
		return -1;
	}
	current = game;

	if (game->Init()) {
		current = nullptr;
		return 1;
	}

	while (true)
	{
		drag_update(game->window);
		for (auto c = game->controllers.begin(); c != game->controllers.end(); c++) {
			(*c)->FrameUpdate();
		}
		if (!game->GameLoop()) {
			break;
		}
	}

	game->Uninit();

	current = nullptr;
	return 0;
}

bool Game::GameLoop()
{
	return !glfwWindowShouldClose(window);
}



//Game::~Game()
//{
//
//}
