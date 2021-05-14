#include <multilua.h>

void util_installfuncs(lua_State* L) {
	// Create the automatic closer:
	lua_pushcfunction(L, multilua_close);
	lua_setfield(L, -2, "__gc");

	// Add the index methods:
	lua_pushcfunction(L, multilua_close);
	lua_setfield(L, -2, "close");

	lua_pushcfunction(L, multilua_openlibs);
	lua_setfield(L, -2, "openlibs");

	lua_pushcfunction(L, multilua_absindex);
	lua_setfield(L, -2, "absindex");

	lua_pushcfunction(L, multilua_arith);
	lua_setfield(L, -2, "arith");

	lua_pushcfunction(L, multilua_current);
	lua_setfield(L, -2, "current");
}

static int multilua_current(lua_State* L) {
	// Otherwise, create our table:
	lua_newtable(L);

	// TODO: Create the metatable...
	if (luaL_newmetatable(L, "multilua_meta")) {
		util_installfuncs(L);

		// Push our actual value:
		lua_pushlightuserdata(L, L);
		lua_setfield(L, -2, "self");
	}
	lua_setmetatable(L, -2);

	// Set the object's index to it's own metatable:
	lua_getmetatable(L, -1);
	lua_getmetatable(L, -2);
	lua_setfield(L, -2, "__index");

	return 1;
}

static int multilua_new(lua_State* L) {
	lua_State* new_state = luaL_newstate();

	// Memory allocation failure:
	if(!new_state) {
		lua_pushnil(L);
		return 1;
	}

	// Otherwise, create our table:
	lua_newtable(L);

	// TODO: Create the metatable...
	if (luaL_newmetatable(L, "multilua_meta")) {
		util_installfuncs(L);

		// Push our actual value:
		lua_pushlightuserdata(L, new_state);
		lua_setfield(L, -2, "self");
	}
	lua_setmetatable(L, -2);

	// Set the object's index to it's own metatable:
	lua_getmetatable(L, -1);
	lua_getmetatable(L, -2);
	lua_setfield(L, -2, "__index");

	return 1;
}

static int multilua_close(lua_State* L) {
	lua_getfield(L, 1, "self");

	// Allow it to be closed multiple times (safely):
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_close(current_state);
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// Set self to nil:
	lua_pushnil(L);
	lua_setfield(L, -2, "self");

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

static int multilua_openlibs(lua_State* L) {
	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		luaL_openlibs(current_state);
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

static int multilua_absindex(lua_State* L) {
	int idx = lua_tointeger(L, 2);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		
		int r = lua_absindex(current_state, idx);
		lua_pushinteger(L, r);
		return 1;
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

static int multilua_arith(lua_State* L) {
	size_t length = 0;
	const char* op = lua_tolstring(L, 2, &length);

	// Catch 0-length string, or invalid string:
	if(length < 1 || op == NULL) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		
		switch(op[0]) {
			case '+':
				lua_arith(L, LUA_OPADD);
				break;
			case '-':
				if(length == 2 && op[1] == 'u') {
					lua_arith(L, LUA_OPUNM);
				} else {
					lua_arith(L, LUA_OPSUB);
				}
				break;
			case '*':
				lua_arith(L, LUA_OPMUL);
				break;
			case '/':
				if(length == 2 && op[1] == '/') {
					lua_arith(L, LUA_OPIDIV);
				} else {
					lua_arith(L, LUA_OPDIV);
				}
				break;
			case '%':
				lua_arith(L, LUA_OPMOD);
				break;
			case '^':
				lua_arith(L, LUA_OPPOW);
				break;
			case '&':
				lua_arith(L, LUA_OPBAND);
				break;
			case '|':
				lua_arith(L, LUA_OPBOR);
				break;
			case '<':
				lua_arith(L, LUA_OPSHL);
				break;
			case '>':
				lua_arith(L, LUA_OPSHR);
				break;
			case '~':
				if(length == 2 && op[1] == '!') {
					lua_arith(L, LUA_OPBNOT);
				} else
				if(length == 2 && op[1] == '|') {
					lua_arith(L, LUA_OPBXOR);
				} else
				{
					lua_pushnil(L);
				}
				break;
			default:
				lua_pushnil(L);
				break;
		}
		
		return 1;
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

// TODO: void lua_call (lua_State *L, int nargs, int nresults);
// TODO: int lua_checkstack (lua_State *L, int n);
// TODO: int lua_compare (lua_State *L, int index1, int index2, int op);
// TODO: void lua_concat (lua_State *L, int n);
// TODO: void lua_copy (lua_State *L, int fromidx, int toidx);
// TODO: void lua_createtable (lua_State *L, int narr, int nrec);
// TODO: int lua_error (lua_State *L);
// TODO: int lua_gc (lua_State *L, int what, int data);
// TODO: int lua_getfield (lua_State *L, int index, const char *k);
// TODO: void *lua_getextraspace (lua_State *L);
// TODO: lua_CFunction lua_atpanic (lua_State *L, lua_CFunction panicf); (?)


LUAMOD_API int luaopen_multilua(lua_State* L) {
	static const struct luaL_Reg multilua [] = {
		{"new", multilua_new},
		{"current", multilua_current},
		{"close", multilua_close},
		{"openlibs", multilua_openlibs},
		{"absindex", multilua_absindex},
		{"arith", multilua_arith},
		{NULL, NULL},
	};

	luaL_newlib(L, multilua);

	// Set the library version
	lua_pushstring(L, MULTILUA_LIB);
	lua_setfield(L, -2, "version");

	return 1;
}
