#include "shell.h"
extern "C" {
#include<lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
extern  lua_State *L;

std::shared_ptr<CameraFPS> camfps;
//std::shared_ptr<Camera> cam;
static int cam_pos(lua_State *L) {
	if (camfps == nullptr) {
		return 0;
	}
	lua_pushnumber(L, camfps->position.x);
	lua_pushnumber(L, camfps->position.y);
	lua_pushnumber(L, camfps->position.z);
	return 3;
}

static int cam_yall_pitch_roll(lua_State *L) {

}

void shell::camera::init_fps(std::shared_ptr<CameraFPS> cam)
{
	camfps = cam;
}
