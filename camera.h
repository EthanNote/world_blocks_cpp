#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <memory>

class Controller;



class PerspectiveInfo {
public:
	float fov;
	float aspect;
	float zNear;
	float zFar;

	PerspectiveInfo();
};


class CCamera: public std::enable_shared_from_this<CCamera> {
private:
	static std::shared_ptr<CCamera> current_attached_camera;
protected:
	glm::mat4 _last_get_projection = glm::mat4(1.0);
	glm::mat4 _last_get_modelview = glm::mat4(1.0);
	glm::mat4 _last_get_mvp = glm::mat4(1.0);

	virtual void setupControls();
	PerspectiveInfo m_perspectiveInfo;


public:


	double sensitivity = 5.0;

	virtual glm::mat4 & GetProjection();
	virtual glm::mat4 & GetModelView();
	virtual glm::mat4 & GetMVP();
	//void attach();
	//void detach();
	//friend static void detachAll();
	std::shared_ptr<Controller> controller=nullptr;
	virtual std::shared_ptr<Controller> CreateController();

	CCamera();


	static std::shared_ptr<CCamera> CreateFPSCamera();
	//virtual ~Camera();
};

typedef std::shared_ptr<CCamera> Camera;


class CFpsCamera :public CCamera {
private:
public:

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	float yall = 0;
	float pitch = 0;
	virtual glm::mat4 & GetModelView() override;
	virtual std::shared_ptr<Controller> CreateController() override;
};

typedef std::shared_ptr<CFpsCamera> FpsCamera;


#endif