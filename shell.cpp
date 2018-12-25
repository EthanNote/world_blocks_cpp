#include "shell.h"

extern "C" {
#include<lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
extern void doREPL(lua_State *L);

static std::thread* idle_thread = NULL;
lua_State *L = NULL;

void shell::init()
{
	L = luaL_newstate();  /* create state */
	if (L == NULL) {
		return;
	}
	luaL_checkversion(L);  /* check that interpreter has correct version */
	luaL_openlibs(L);  /* open standard libraries */
	//print_version();
	lua_writestring(LUA_COPYRIGHT, strlen(LUA_COPYRIGHT));
	lua_writeline();
}

void shell::destroy()
{
}


int shell::start_idle()
{
	idle_thread = new std::thread([&] {		
		doREPL(L);  /* do read-eval-print loop */
		lua_close(L);
	});

	return 0;
}

