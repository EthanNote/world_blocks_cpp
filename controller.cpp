#include<GLFW/glfw3.h>
#include "controller.h"
#include "camera.h"

struct MOUSE_DRAG_CONTROL {

	bool is_active;

	double dx;
	double dy;

	double _last_x;
	double _last_y;
};

struct MOUSE_DRAG_CONTROL drag_control;

void drag_init(GLFWwindow* window) {

	drag_control.is_active = true;
	drag_control.dx = 0;
	drag_control.dy = 0;
	glfwGetCursorPos(window, &drag_control._last_x, &drag_control._last_y);
}

void drag_stop() {
	drag_control.is_active = false;
	drag_control.dx = 0;
	drag_control.dy = 0;
}

void drag_update(GLFWwindow* window) {
	if (!drag_control.is_active) {
		return;
	}
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	drag_control.dx = x - drag_control._last_x;
	drag_control.dy = y - drag_control._last_y;
	drag_control._last_x = x;
	drag_control._last_y = y;
}


void drag_get_vector(double *dx, double *dy) {
	*dx = drag_control.dx;
	*dy = drag_control.dy;
}

std::shared_ptr<Controller> Controller::getptr()
{
	return shared_from_this();
}

void Controller::FrameUpdate()
{
}

void CameraFPSController::FrameUpdate()
{
	auto camera = std::dynamic_pointer_cast<CFpsCamera, CCamera>(this->camera);
	double dx, dy;
	drag_get_vector(&dx, &dy);
	camera->yall += dx / camera->sensitivity;
	camera->pitch += dy / camera->sensitivity;
}

//void ControllerAxis::TestKey(int key)
//{
//	for (int i = 0; i < 4; i++) {
//		auto iter = std::find(stroke_keys[i].begin(), stroke_keys[i].end(), key);
//		if (iter != stroke_keys[i].end()) {
//			stroke_strength[i] = 1;
//			break;
//		}
//	}
//}



void InputAxis::TestKey(int key, int action)
{
	for (int i = 0; i < 4; i++) {
		auto iter = std::find(stroke_keys[i].begin(), stroke_keys[i].end(), key);
		if (iter != stroke_keys[i].end()) {
			stroke_strength[i] = action;
			break;
		}
	}
}

void InputAxis::OnKeyEvent(int key, int scancode, int action, int mods)
{
	TestKey(key, action);
}

InputAxis::InputAxis()
{
	stroke_keys[UP].push_back(GLFW_KEY_W);
	stroke_keys[UP].push_back(GLFW_KEY_UP);

	stroke_keys[DOWN].push_back(GLFW_KEY_S);
	stroke_keys[DOWN].push_back(GLFW_KEY_DOWN);
	
	stroke_keys[LEFT].push_back(GLFW_KEY_A);
	stroke_keys[LEFT].push_back(GLFW_KEY_LEFT);
	
	stroke_keys[RIGHT].push_back(GLFW_KEY_D);
	stroke_keys[RIGHT].push_back(GLFW_KEY_RIGHT);
}

float InputAxis::GetX()
{
	return stroke_strength[RIGHT] - stroke_strength[LEFT];
}

float InputAxis::GetY()
{
	return stroke_strength[UP] - stroke_strength[DOWN];
}

#include<list>
std::list<KeyEventHandler*> handlers;

void keyfunc(GLFWwindow*, int key, int scancode, int action, int mods) {
	_handler_call(key, scancode, action, mods);
}

void _handler_call(int key, int scancode, int action, int mods) {
	for (auto i = handlers.begin(); i != handlers.end(); i++) {
		(*i)->OnKeyEvent(key, scancode, action, mods);
	}
}

void KeyEventHandler::Enable()
{
	GLFWkeyfun(keyfunc);
}

KeyEventHandler::KeyEventHandler()
{
	handlers.push_back(this);
	
}

KeyEventHandler::~KeyEventHandler()
{
	handlers.remove(this);
}
