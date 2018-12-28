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
