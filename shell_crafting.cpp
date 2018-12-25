#include "shell.h"

extern "C" {
#include<lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
extern  lua_State *L;
BlockTree g_tree;

static int lib_tree_size(lua_State *L) {
	int size = 0;
	if (g_tree != nullptr) {
		size = g_tree->pool->blocks.size();
	}
	lua_pushinteger(L, size);
	return 1;
}

static int lib_tree_insert(lua_State *L) {
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int z = luaL_checkinteger(L, 3);
	int level = luaL_checkinteger(L, 4);
	Block b;
	memset(&b, 0, sizeof(b));
	b.x = x;
	b.y = y;
	b.z = z;
	b.level = level;
	b.color = 0;
	g_tree->Insert(b);
	lua_pushinteger(L, g_tree->pool->blocks.size());

	return 1;
}




void shell::crafting::init(BlockTree tree)
{
	g_tree = tree;
	static const struct luaL_Reg funcs[] = {
		{"tree_size", lib_tree_size},
		{"tree_insert", lib_tree_insert},
		{NULL, NULL},
	};

	lua_getglobal(L, "crafting");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
	}
	luaL_setfuncs(L, funcs, 0);
	lua_setglobal(L, "crafting");
	
}