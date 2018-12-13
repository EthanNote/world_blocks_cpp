#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include<memory>
#include<vector>

class Camera;

class Controller :std::enable_shared_from_this<Controller> {
public:
	std::shared_ptr<Controller> getptr();

	virtual void FrameUpdate();
};

class CameraController : public Controller {
public:
	std::shared_ptr<Camera> camera;
};


class CameraFPSController : public CameraController {
	virtual void FrameUpdate() override;
};



#endif // !_CONTROLLER_H_