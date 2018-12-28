#include<memory>
#include "camera.h"
#include "controller.h"

std::shared_ptr<CCamera> CCamera::current_attached_camera = nullptr;

glm::mat4 & CCamera::GetProjection()
{
	this->_last_get_projection =
		glm::perspective(
			glm::radians(this->m_perspectiveInfo.fov),
			this->m_perspectiveInfo.aspect,
			this->m_perspectiveInfo.zNear,
			this->m_perspectiveInfo.zFar);

	return this->_last_get_projection;
}

glm::mat4 & CCamera::GetModelView()
{
	return this->_last_get_modelview;
}

glm::mat4 & CCamera::GetMVP()
{
	this->_last_get_mvp = this->GetProjection() * this->GetModelView();
	return this->_last_get_mvp;
}


void CCamera::setupControls()
{
}

//void Camera::attach()
//{
//	current_attached_camera = std::shared_ptr<Camera>(this);
//}
//
//void Camera::detach()
//{
//	if (current_attached_camera == std::shared_ptr<Camera>(this)) {
//		current_attached_camera = nullptr;
//	}
//}

std::shared_ptr<Controller> CCamera::CreateController()
{
	auto controller = std::shared_ptr<Controller>(new Controller());
	return controller;
}

CCamera::CCamera()
{
}

std::shared_ptr<CCamera> CCamera::CreateFPSCamera()
{
	return std::shared_ptr<CFpsCamera>(new CFpsCamera);
}

//Camera::~Camera()
//{
//	//this->detach();
//}

//void detachAll() {
//	Camera::current_attached_camera = nullptr;
//}


glm::mat4 & CFpsCamera::GetModelView()
{
	float ryall = glm::radians(this->yall);
	float rpitch = glm::radians(this->pitch);
	glm::vec3 looktarget(
		-sin(ryall)*cos(rpitch) + this->position.x,
		sin(rpitch) + this->position.y,
		-cos(ryall)*cos(rpitch) + this->position.z);
	auto mv = glm::lookAt(this->position, looktarget, glm::vec3(0, 1, 0));
	this->_last_get_modelview = mv;

	return _last_get_modelview;
}

std::shared_ptr<Controller> CFpsCamera::CreateController()
{
	std::shared_ptr<CameraFPSController> controller = std::shared_ptr<CameraFPSController>(new CameraFPSController);
	controller->camera = shared_from_this();

	return std::static_pointer_cast<Controller, CameraFPSController>(controller);
}

//glm::mat4 & CameraFPS::GetMVP()
//{
//	/*float ryall = glm::radians(this->yall);
//	float rpitch = glm::radians(this->pitch);
//	glm::vec3 looktarget(
//		-sin(ryall)*cos(rpitch) + this->position.x,
//					sin(rpitch) + this->position.y,
//		-cos(ryall)*cos(rpitch) + this->position.z);
//	auto mv = glm::lookAt(this->position, looktarget, glm::vec3(0, 1, 0));
//	this->_last_get_modelview = mv;*/
//
//
//	/*auto p = glm::perspective(glm::radians(60.0f), 1.33f, 0.1f, 10.0f);
//	this->_last_get_projection = p;*/
//
//	this->_last_get_mvp = this->GetProjection() * this->GetModelView();
//	return this->_last_get_mvp;
//}



PerspectiveInfo::PerspectiveInfo()
{
	this->fov = 60;
	this->aspect = 1.33;
	this->zFar = 1000;
	this->zNear = 0.1;
}
