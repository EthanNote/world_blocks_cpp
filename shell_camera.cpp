#include "shell.h"
extern "C" {
#include<lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
extern  lua_State *L;

std::shared_ptr<CFpsCamera> camfps;
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
	if (camfps == nullptr) {
		return 0;
	}
	lua_pushnumber(L, camfps->yall);
	lua_pushnumber(L, camfps->pitch);
	lua_pushnumber(L, 0);
	return 3;
}

void shell::camera::init_fps(std::shared_ptr<CFpsCamera> cam)
{
	camfps = cam;
	static const struct luaL_Reg funcs[] = {
		{ "pos", cam_pos },
		{ "yall_pitch_roll", cam_yall_pitch_roll },
		{ NULL, NULL },
	};

	lua_getglobal(L, "camera");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
	}
	luaL_setfuncs(L, funcs, 0);
	lua_setglobal(L, "camera");
}
