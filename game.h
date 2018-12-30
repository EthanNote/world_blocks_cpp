#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "controller.h"

#include <memory>
#include <list>

class Game
{

private:

protected:
	std::list<std::shared_ptr<FrameEventHandler>> controllers;

	GLFWwindow* window;
	virtual int Init();
	virtual int Uninit();

public:
	//std::shared_ptr<Game> GetCurrent();
	static int Run(std::shared_ptr<Game> game);
	virtual bool GameLoop();
	//Game();
	//~Game();
};

