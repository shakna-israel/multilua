#include <stdbool.h>
#include <string.h>

#include <multilua.h>

#define LUA_TYPE_HERE(_L) lua_typename(_L, lua_type(_L, -1))

void util_installfuncs(lua_State* L) {
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

	lua_pushcfunction(L, multilua_call);
	lua_setfield(L, -2, "call");

	lua_pushcfunction(L, multilua_checkstack);
	lua_setfield(L, -2, "checkstack");

	lua_pushcfunction(L, multilua_compare);
	lua_setfield(L, -2, "compare");

	lua_pushcfunction(L, multilua_concat);
	lua_setfield(L, -2, "concat");

	lua_pushcfunction(L, multilua_copy);
	lua_setfield(L, -2, "copy");

	lua_pushcfunction(L, multilua_createtable);
	lua_setfield(L, -2, "createtable");

	lua_pushcfunction(L, multilua_error);
	lua_setfield(L, -2, "error");

	lua_pushcfunction(L, multilua_gc);
	lua_setfield(L, -2, "gc");

	lua_pushcfunction(L, multilua_getfield);
	lua_setfield(L, -2, "getfield");

	lua_pushcfunction(L, multilua_luaversion);
	lua_setfield(L, -2, "luaversion");

	lua_pushcfunction(L, multilua_xmove);
	lua_setfield(L, -2, "xmove");

	lua_pushcfunction(L, multilua_yield);
	lua_setfield(L, -2, "yield");

	lua_pushcfunction(L, multilua_getglobal);
	lua_setfield(L, -2, "getglobal");

	lua_pushcfunction(L, multilua_geti);
	lua_setfield(L, -2, "geti");

	lua_pushcfunction(L, multilua_getmetatable);
	lua_setfield(L, -2, "getmetatable");

	lua_pushcfunction(L, multilua_gettable);
	lua_setfield(L, -2, "gettable");

	lua_pushcfunction(L, multilua_gettop);
	lua_setfield(L, -2, "gettop");

	lua_pushcfunction(L, multilua_getuservalue);
	lua_setfield(L, -2, "getuservalue");

	lua_pushcfunction(L, multilua_insert);
	lua_setfield(L, -2, "insert");

	lua_pushcfunction(L, multilua_isboolean);
	lua_setfield(L, -2, "isboolean");

	lua_pushcfunction(L, multilua_iscfunction);
	lua_setfield(L, -2, "iscfunction");

	lua_pushcfunction(L, multilua_isfunction);
	lua_setfield(L, -2, "isfunction");

	lua_pushcfunction(L, multilua_isinteger);
	lua_setfield(L, -2, "isinteger");

	lua_pushcfunction(L, multilua_islightuserdata);
	lua_setfield(L, -2, "islightuserdata");

	lua_pushcfunction(L, multilua_isnil);
	lua_setfield(L, -2, "isnil");

	lua_pushcfunction(L, multilua_isnone);
	lua_setfield(L, -2, "isnone");

	lua_pushcfunction(L, multilua_isnoneornil);
	lua_setfield(L, -2, "isnoneornil");

	lua_pushcfunction(L, multilua_isnumber);
	lua_setfield(L, -2, "isnumber");

	lua_pushcfunction(L, multilua_isstring);
	lua_setfield(L, -2, "isstring");

	lua_pushcfunction(L, multilua_istable);
	lua_setfield(L, -2, "istable");

	lua_pushcfunction(L, multilua_isthread);
	lua_setfield(L, -2, "isthread");

	lua_pushcfunction(L, multilua_isuserdata);
	lua_setfield(L, -2, "isuserdata");

	lua_pushcfunction(L, multilua_isyieldable);
	lua_setfield(L, -2, "isyieldable");

	lua_pushcfunction(L, multilua_len);
	lua_setfield(L, -2, "len");

	lua_pushcfunction(L, multilua_newtable);
	lua_setfield(L, -2, "newtable");

	lua_pushcfunction(L, multilua_newthread);
	lua_setfield(L, -2, "newthread");

	lua_pushcfunction(L, multilua_newuserdata);
	lua_setfield(L, -2, "newuserdata");

	lua_pushcfunction(L, multilua_next);
	lua_setfield(L, -2, "next");

	lua_pushcfunction(L, multilua_pcall);
	lua_setfield(L, -2, "pcall");

	lua_pushcfunction(L, multilua_pop);
	lua_setfield(L, -2, "pop");

	lua_pushcfunction(L, multilua_pushboolean);
	lua_setfield(L, -2, "pushboolean");

	lua_pushcfunction(L, multilua_pushglobaltable);
	lua_setfield(L, -2, "pushglobaltable");

	lua_pushcfunction(L, multilua_pushinteger);
	lua_setfield(L, -2, "pushinteger");

	lua_pushcfunction(L, multilua_pushstring);
	lua_setfield(L, -2, "pushstring");

	lua_pushcfunction(L, multilua_pushnstring);
	lua_setfield(L, -2, "pushnstring");

	lua_pushcfunction(L, multilua_pushnil);
	lua_setfield(L, -2, "pushnil");

	lua_pushcfunction(L, multilua_pushnumber);
	lua_setfield(L, -2, "pushnumber");

	lua_pushcfunction(L, multilua_pushvalue);
	lua_setfield(L, -2, "pushvalue");

	lua_pushcfunction(L, multilua_pushlightuserdata);
	lua_setfield(L, -2, "pushlightuserdata");

	lua_pushcfunction(L, multilua_rawequal);
	lua_setfield(L, -2, "rawequal");

	lua_pushcfunction(L, multilua_rawget);
	lua_setfield(L, -2, "rawget");

	lua_pushcfunction(L, multilua_rawgeti);
	lua_setfield(L, -2, "rawgeti");

	lua_pushcfunction(L, multilua_rawlen);
	lua_setfield(L, -2, "rawlen");

	lua_pushcfunction(L, multilua_rawset);
	lua_setfield(L, -2, "rawset");

	lua_pushcfunction(L, multilua_rawseti);
	lua_setfield(L, -2, "rawseti");

	lua_pushcfunction(L, multilua_rawsetp);
	lua_setfield(L, -2, "rawsetp");
}

void util_installmeta(lua_State* L) {
	// Create the comparator:
	lua_getmetatable(L, -1);
	lua_pushcfunction(L, multilua_equal);
	lua_setfield(L, -2, "__eq");

	// Create the automatic closer:
	lua_getmetatable(L, -1);
	lua_pushcfunction(L, multilua_close);
	lua_setfield(L, -2, "__gc");
}

static int multilua_equal(lua_State* L) {
	// 1 - multilua stateA
	// 2 - multilua stateB

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* first_state = lua_touserdata(L, -1);

		lua_getfield(L, 2, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* second_state = lua_touserdata(L, -1);

			if(first_state == second_state) {
				lua_pushboolean(L, true);
			} else {
				lua_pushboolean(L, false);
			}
			return 1;
		}

		lua_pushnil(L);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_current(lua_State* L) {
	// Otherwise, create our table:
	lua_newtable(L);

	// Create the metatable...
	lua_newtable(L);
	util_installfuncs(L);
	// Set the metatable
	lua_setmetatable(L, -2);

	// Set the object's index to it's own metatable:
	lua_getmetatable(L, -1);
	lua_getmetatable(L, -2);
	lua_setfield(L, -2, "__index");

	util_installmeta(L);

	// Push our actual value:
	lua_pushlightuserdata(L, L);
	lua_setfield(L, -2, "self");

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

	// Create the metatable...
	lua_newtable(L);
	util_installfuncs(L);
	// Set the metatable
	lua_setmetatable(L, -2);

	// Set the object's index to it's own metatable:
	lua_getmetatable(L, -1);
	lua_getmetatable(L, -2);
	lua_setfield(L, -2, "__index");

	util_installmeta(L);

	// Push our actual value:
	lua_pushlightuserdata(L, new_state);
	lua_setfield(L, -2, "self");

	return 1;
}

static int multilua_close(lua_State* L) {
	lua_getfield(L, 1, "self");

	// Allow it to be closed multiple times (safely):
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		// Don't close the host Lua pointer!
		if(current_state != L) {
			lua_close(current_state);
		}
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

static int multilua_call(lua_State* L) {
	int nargs_bool = false;
	int nargs = lua_tointegerx(L, 2, &nargs_bool);

	if(!nargs_bool) {
		lua_pushnil(L);
		return 1;
	}

	int nresults_bool = false;
	int nresults = lua_tointegerx(L, 3, &nresults_bool);

	if(!nresults_bool) {
		nresults = LUA_MULTRET;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_call(current_state, nargs, nresults);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checkstack(lua_State* L) {
	int n_bool = false;
	int n = lua_tointegerx(L, 2, &n_bool);

	if(!n_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_checkstack(current_state, n);

		lua_pushboolean(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_compare(lua_State* L) {
	// 1 - multilua state
	// 2 - index1
	// 3 - index2
	// 4 - op
	// "=="
	// "<"
	// "<="

	int index1_bool = false;
	int index1 = lua_tointegerx(L, 2, &index1_bool);

	if(!index1_bool) {
		lua_pushnil(L);
		return 1;
	}

	int index2_bool = false;
	int index2 = lua_tointegerx(L, 3, &index2_bool);

	if(!index2_bool) {
		lua_pushnil(L);
		return 1;
	}

	size_t length = 0;
	const char* op = lua_tolstring(L, 4, &length);

	if(length == 0 || op == NULL) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = false;

		switch(op[0]) {
			case '=':
				r = lua_compare(current_state, index1, index2, LUA_OPEQ);
				lua_pushboolean(L, r);
				break;
			case '<':
				if(length == 2 && op[1] == '=') {
					r = lua_compare(current_state, index1, index2, LUA_OPLE);
				} else {
					r = lua_compare(current_state, index1, index2, LUA_OPLT);
				}
				lua_pushboolean(L, r);
				break;
			default:
				lua_pushnil(L);
				break;
		}

		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_concat(lua_State* L) {
	int n_bool = false;
	int n = lua_tointegerx(L, 2, &n_bool);

	if(!n_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_concat(current_state, n);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_copy(lua_State* L) {
	// 1 - metalua state
	// 2 - fromidx
	// 3 - toidx

	int fromidx_bool = false;
	int fromidx = lua_tointegerx(L, 2, &fromidx_bool);

	if(!fromidx_bool) {
		lua_pushnil(L);
		return 1;
	}

	int toidx_bool = false;
	int toidx = lua_tointegerx(L, 3, &toidx_bool);

	if(!toidx_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_copy(current_state, fromidx, toidx);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_createtable(lua_State* L) {
	// 1 - multilua state
	// 2- narr
	// 3 - nrec

	int narr_bool = false;
	int narr = lua_tointegerx(L, 2, &narr_bool);
	if(!narr_bool) {
		narr = 0;
	}

	int nrec_bool = false;
	int nrec = lua_tointegerx(L, 3, &nrec_bool);
	if(!nrec_bool) {
		nrec = 0;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_createtable(current_state, narr, nrec);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_error(lua_State* L) {
	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_error(current_state);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_gc(lua_State* L) {
	// 1 - multilua state
	// 2 - what
	// 3 - data

	int data_bool = false;
	int data = lua_tointegerx(L, 3, &data_bool);

	size_t what_length = 0;
	const char* what = lua_tolstring(L, 2, &what_length);

	// Default mode - collect
	if(what == NULL || what_length == 0) {
		what_length = 8;
		what = "collect";
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		if(strncmp(what, "collect", what_length) == 0) {
			lua_gc(current_state, LUA_GCCOLLECT, data);
			lua_pushboolean(L, true);
			return 1;
		} else
		if(strncmp(what, "stop", what_length) == 0) {
			lua_gc(current_state, LUA_GCSTOP, data);
			lua_pushboolean(L, true);
			return 1;
		} else
		if(strncmp(what, "restart", what_length) == 0) {
			lua_gc(current_state, LUA_GCRESTART, data);
			lua_pushboolean(L, true);
			return 1;
		} else
		if(strncmp(what, "count", what_length) == 0) {
			int kb = lua_gc(current_state, LUA_GCCOUNT, data);
			int bt = lua_gc(current_state, LUA_GCCOUNTB, data);

			lua_pushinteger(L, kb);
			lua_pushinteger(L, bt);
			return 2;
		} else
		if(strncmp(what, "step", what_length) == 0) {
			lua_gc(current_state, LUA_GCSTEP, data);

			lua_pushboolean(L, true);
			return 1;
		} else
		if(strncmp(what, "setpause", what_length) == 0) {
			int prev_pause = lua_gc(current_state, LUA_GCSETPAUSE, data);

			lua_pushinteger(L, prev_pause);
			return 1;
		} else
		if(strncmp(what, "setstepmul", what_length) == 0) {
			int prev_stepmul = lua_gc(current_state, LUA_GCSETSTEPMUL, data);

			lua_pushinteger(L, prev_stepmul);
			return 1;
		} else
		if(strncmp(what, "isrunning", what_length) == 0) {
			int running = lua_gc(current_state, LUA_GCISRUNNING, data);

			lua_pushboolean(L, running);
			return 1;
		} else {
			lua_pushnil(L);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getfield(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - k

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);

	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	const char* k = lua_tostring(L, 3);
	if(k == NULL) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int type = lua_getfield(current_state, index, k);

		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_luaversion(lua_State* L) {
	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		const lua_Number* ver = lua_version(current_state);

		lua_pushnumber(L, *ver);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_xmove(lua_State* L) {
	// 1 - multilua state from
	// 2 - multilua state to
	// 3 - n
	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* from_state = lua_touserdata(L, -1);

		lua_getfield(L, 2, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* to_state = lua_touserdata(L, -1);

			int n_bool = false;
			int n = lua_tointegerx(L, 3, &n_bool);
			if(!n_bool) {
				lua_pushnil(L);
				return 1;
			}

			lua_xmove(from_state, to_state, n);
			lua_pushboolean(L, true);
			return 1;
		}
		else {
			lua_pushnil(L);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_yield(lua_State* L) {
	// 1 - multilua state
	// 2 - nresults

	int nresults_bool = false;
	int nresults = lua_tointegerx(L, 2, &nresults_bool);

	if(!nresults_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_yield(current_state, nresults);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getglobal(lua_State* L) {
	// 1 - multilua state
	// 2 - name

	const char* name = lua_tostring(L, 2);
	if(!name) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_getglobal(current_state, name);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_geti(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - i

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);

	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	int i_bool = false;
	lua_Integer i = lua_tointegerx(L, 3, &i_bool);

	if(!i_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int type = lua_geti(current_state, index, i);

		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getmetatable(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_getmetatable(current_state, index);

		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_gettable(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int type = lua_gettable(current_state, index);

		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_gettop(lua_State* L) {
	// 1 - multilua state

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int index = lua_gettop(current_state);
		lua_pushinteger(L, index);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getuservalue(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int type = lua_getuservalue(L, index);
		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_insert(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_insert(current_state, index);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isboolean(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isboolean(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_iscfunction(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_iscfunction(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isfunction(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isfunction(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isinteger(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isinteger(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_islightuserdata(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_islightuserdata(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isnil(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isnil(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isnone(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isnone(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isnoneornil(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isnoneornil(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isnumber(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isnumber(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isstring(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isstring(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_istable(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_istable(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isthread(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isthread(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isuserdata(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isuserdata(current_state, index);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_isyieldable(lua_State* L) {
	// 1 - multistate lua

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_isyieldable(current_state);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_len(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);

	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_len(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newtable(lua_State* L) {
	// 1 - multilua state

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_newtable(current_state);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newthread(lua_State* L) {
	// 1 - multilua state

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_State* thread = lua_newthread(current_state);
		if(!thread) {
			lua_pushnil(L);
			return 1;
		}

		lua_pushlightuserdata(current_state, thread);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newuserdata(lua_State* L) {
	// 1 - multilua state
	// 2 - size

	int bool_size = false;
	size_t size = lua_tointegerx(L, 2, &bool_size);
	if(!bool_size) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		void* ptr = lua_newuserdata(current_state, size);

		lua_pushlightuserdata(L, ptr);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_next(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int num = lua_next(current_state, index);
		
		lua_pushinteger(L, num);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pcall(lua_State* L) {
	// 1 - multilua state
	// 2 - nargs
	// 3 - nresults
	// 4 - msgh

	int nargs_bool = false;
	int nargs = lua_tointegerx(L, 2, &nargs_bool);
	if(!nargs_bool) {
		lua_pushnil(L);
		return 1;
	}

	int nresults_bool = false;
	int nresults = lua_tointegerx(L, 3, &nresults_bool);
	if(!nresults_bool) {
		lua_pushnil(L);
		return 1;
	}

	int bool_msgh = false;
	int msgh = lua_tointegerx(L, 4, &bool_msgh);
	if(!bool_msgh) {
		msgh = 0;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_pcall(current_state, nargs, nresults, msgh);

		switch(r) {
			case LUA_OK:
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
				break;
			case LUA_ERRRUN:
				lua_pushnil(L);
				lua_pushstring(L, "runtime");
				break;
			case LUA_ERRMEM:
				lua_pushnil(L);
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRERR:
				lua_pushnil(L);
				lua_pushstring(L, "error");
				break;
			case LUA_ERRGCMM:
				lua_pushnil(L);
				lua_pushstring(L, "gcmeta");
				break;
			default:
				lua_pushnil(L);
				lua_pushstring(L, "other");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pop(lua_State* L) {
	// 1 - multilua state
	// 2 - n

	int bool_n = false;
	int n = lua_tointegerx(L, 2, &bool_n);
	if(!n) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pop(current_state, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushboolean(lua_State* L) {
	// 1 - multilua state
	// 2 - b

	int b_bool = false;
	int b = lua_tointegerx(L, 2, &b_bool);
	if(!b_bool) {
		b = false;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushboolean(current_state, b);
		lua_pushboolean(L, true);
		return 1;
	}


	lua_pushnil(L);
	return 1;
}

static int multilua_pushglobaltable(lua_State* L) {
	// 1 - multilua state

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushglobaltable(current_state);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushinteger(lua_State* L) {
	// 1 - multilua state
	// 2 - n

	int n_bool = false;
	lua_Integer n = lua_tointegerx(L, 2, &n_bool);
	if(!n_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushinteger(current_state, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushstring(lua_State* L) {
	// 1 - multilua state
	// 2 - s

	size_t length = 0;
	const char* s = lua_tolstring(L, 2, &length);
	if(!s) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushlstring(current_state, s, length);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushnstring(lua_State* L) {
	// 1 - multilua state
	// 2 - s
	// 3 - maxlength

	int maxlength_bool = false;
	lua_Integer maxlength = lua_tointegerx(L, 3, &maxlength_bool);

	if(!maxlength_bool) {
		lua_pushnil(L);
		return 1;
	}

	size_t length = 0;
	const char* s = lua_tolstring(L, 2, &length);
	if(!s) {
		lua_pushnil(L);
		return 1;
	}

	if(length > maxlength) {
		length = maxlength;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushlstring(current_state, s, length);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushnil(lua_State* L) {
	// 1 - multilua state

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushnil(current_state);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushnumber(lua_State* L) {
	// 1 - multilua state
	// 2 - n

	int n_bool = false;
	lua_Number n = lua_tonumberx(L, 2, &n_bool);
	if(!n_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushnumber(current_state, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushvalue(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushvalue(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushlightuserdata(lua_State* L) {
	// 1 - multilua state
	// 2 - lightuserdata

	if(!lua_islightuserdata(L, 2)) {
		lua_pushnil(L);
		return 1;
	}

	void* ptr = lua_touserdata(L, 2);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_pushlightuserdata(current_state, ptr);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawequal(lua_State* L) {
	// 1 - multilua state
	// 2 - indexA
	// 3 - indexB

	int bool_indexA = false;
	int indexA = lua_tointegerx(L, 2, &bool_indexA);
	if(!bool_indexA) {
		lua_pushnil(L);
		return 1;
	}

	int bool_indexB = false;
	int indexB = lua_tointegerx(L, 3, &bool_indexB);
	if(!bool_indexB) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int r = lua_rawequal(current_state, indexA, indexB);
		if(r == 1) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawget(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int type = lua_rawget(current_state, index);
		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawgeti(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - n

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	int bool_n = false;
	lua_Integer n = lua_tointegerx(L, 3, &bool_n);
	if(!bool_n) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int type = lua_rawgeti(current_state, index, n);
		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawlen(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int bool_index = false;
	int index = lua_tointegerx(L, -2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		size_t len = lua_rawlen(current_state, index);
		lua_pushinteger(L, len);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawset(lua_State* L) {
	// 1 - multilua state
	// 2 - index

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_rawset(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawseti(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - i

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	int bool_i = false;
	lua_Integer i = lua_tointegerx(L, 3, &bool_i);
	if(!bool_i) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_rawseti(current_state, index, i);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawsetp(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - pointer

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	if(lua_islightuserdata(L, 3)) {
		const void* ptr = lua_touserdata(L, 3);

		lua_getfield(L, 1, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* current_state = lua_touserdata(L, -1);
			
			lua_rawsetp(current_state, index, ptr);

			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

// TODO: void lua_remove (lua_State *L, int index);
// TODO: void lua_replace (lua_State *L, int index);
// TODO: int lua_resume (lua_State *L, lua_State *from, int nargs);
// TODO: void lua_rotate (lua_State *L, int idx, int n);
// TODO: void lua_setfield (lua_State *L, int index, const char *k);
// TODO: void lua_setglobal (lua_State *L, const char *name);
// TODO: void lua_seti (lua_State *L, int index, lua_Integer n);
// TODO: void lua_setmetatable (lua_State *L, int index);
// TODO: void lua_settable (lua_State *L, int index);
// TODO: void lua_settop (lua_State *L, int index);
// TODO: void lua_setuservalue (lua_State *L, int index);
// TODO: int lua_status (lua_State *L);
// TODO: size_t lua_stringtonumber (lua_State *L, const char *s);
// TODO: int lua_toboolean (lua_State *L, int index);
// TODO: lua_Integer lua_tointeger (lua_State *L, int index);
// TODO: lua_Integer lua_tointegerx (lua_State *L, int index, int *isnum);
// TODO: const char *lua_tolstring (lua_State *L, int index, size_t *len);
// TODO: lua_Number lua_tonumber (lua_State *L, int index);
// TODO: lua_Number lua_tonumberx (lua_State *L, int index, int *isnum);
// TODO: const void *lua_topointer (lua_State *L, int index);
// TODO: const char *lua_tostring (lua_State *L, int index);
// TODO: lua_State *lua_tothread (lua_State *L, int index);
// TODO: void *lua_touserdata (lua_State *L, int index);
// TODO: int lua_type (lua_State *L, int index);
// TODO: const char *lua_typename (lua_State *L, int tp);
// TODO: int lua_upvalueindex (int i);
// TODO: int lua_gethookcount (lua_State *L);
// TODO: int lua_gethookmask (lua_State *L);
// TODO: const char *lua_getupvalue (lua_State *L, int funcindex, int n);
// TODO: const char *lua_setupvalue (lua_State *L, int funcindex, int n);
// TODO: void *lua_upvalueid (lua_State *L, int funcindex, int n);
// TODO: void lua_upvaluejoin (lua_State *L, int funcindex1, int n1, int funcindex2, int n2);
// TODO: void luaL_argcheck (lua_State *L, int cond, int arg, const char *extramsg);
// TODO: int luaL_argerror (lua_State *L, int arg, const char *extramsg);
// TODO: int luaL_callmeta (lua_State *L, int obj, const char *e);
// TODO: void luaL_checkany (lua_State *L, int arg);
// TODO: lua_Integer luaL_checkinteger (lua_State *L, int arg);
// TODO: const char *luaL_checklstring (lua_State *L, int arg, size_t *l);
// TODO: lua_Number luaL_checknumber (lua_State *L, int arg);
// TODO: void luaL_checkstack (lua_State *L, int sz, const char *msg);
// TODO: const char *luaL_checkstring (lua_State *L, int arg);
// TODO: void luaL_checktype (lua_State *L, int arg, int t);
// TODO: void *luaL_checkudata (lua_State *L, int arg, const char *tname);
// TODO: void luaL_checkversion (lua_State *L);
// TODO: int luaL_dofile (lua_State *L, const char *filename);
// TODO: int luaL_dostring (lua_State *L, const char *str);
// TODO: int luaL_execresult (lua_State *L, int stat);
// TODO: int luaL_fileresult (lua_State *L, int stat, const char *fname);
// TODO: int luaL_getmetafield (lua_State *L, int obj, const char *e);
// TODO: int luaL_getmetatable (lua_State *L, const char *tname);
// TODO: int luaL_getsubtable (lua_State *L, int idx, const char *fname);
// TODO: const char *luaL_gsub (lua_State *L, const char *s, const char *p, const char *r);
// TODO: lua_Integer luaL_len (lua_State *L, int index);
// TODO: int luaL_loadbuffer (lua_State *L, const char *buff, size_t sz, const char *name);
// TODO: int luaL_loadbufferx (lua_State *L, const char *buff, size_t sz, const char *name, const char *mode);
// TODO: int luaL_loadfile (lua_State *L, const char *filename);
// TODO: int luaL_loadfilex (lua_State *L, const char *filename, const char *mode);
// TODO: int luaL_loadstring (lua_State *L, const char *s);
// TODO: int luaL_newmetatable (lua_State *L, const char *tname);
// TODO: lua_Integer luaL_optinteger (lua_State *L, int arg, lua_Integer d);
// TODO: const char *luaL_optlstring (lua_State *L, int arg, const char *d, size_t *l);
// TODO: lua_Number luaL_optnumber (lua_State *L, int arg, lua_Number d);
// TODO: const char *luaL_optstring (lua_State *L, int arg, const char *d);
// TODO: int luaL_ref (lua_State *L, int t);
// TODO: void luaL_setmetatable (lua_State *L, const char *tname);
// TODO: void *luaL_testudata (lua_State *L, int arg, const char *tname);
// TODO: const char *luaL_tolstring (lua_State *L, int idx, size_t *len);
// TODO: void luaL_traceback (lua_State *L, lua_State *L1, const char *msg, int level);
// TODO: const char *luaL_typename (lua_State *L, int index);
// TODO: void luaL_unref (lua_State *L, int t, int ref);
// TODO: void luaL_where (lua_State *L, int lvl);

// These are slightly harder to wrap:
// TODO: int lua_rawgetp (lua_State *L, int index, const void *p);
// TODO: int lua_pushthread (lua_State *L);
// TODO: void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);
// TODO: void lua_pushcfunction (lua_State *L, lua_CFunction f);
// TODO: void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup);
// TODO: void luaL_requiref (lua_State *L, const char *modname, lua_CFunction openf, int glb);
// TODO: void luaL_pushresultsize (luaL_Buffer *B, size_t sz);
// TODO: void luaL_pushresult (luaL_Buffer *B);
// TODO: char *luaL_prepbuffsize (luaL_Buffer *B, size_t sz);
// TODO: char *luaL_prepbuffer (luaL_Buffer *B);
// TODO: T luaL_opt (L, func, arg, dflt);
// TODO: void luaL_newlibtable (lua_State *L, const luaL_Reg l[]);
// TODO: void luaL_newlib (lua_State *L, const luaL_Reg l[]);
// TODO: int luaL_error (lua_State *L, const char *fmt, ...);
// TODO: int luaL_checkoption (lua_State *L, int arg, const char *def, const char *const lst[]);
// TODO: char *luaL_buffinitsize (lua_State *L, luaL_Buffer *B, size_t sz);
// TODO: void luaL_buffinit (lua_State *L, luaL_Buffer *B);
// TODO: void luaL_addchar (luaL_Buffer *B, char c);
// TODO: void luaL_addlstring (luaL_Buffer *B, const char *s, size_t l);
// TODO: void luaL_addsize (luaL_Buffer *B, size_t n);
// TODO: void luaL_addstring (luaL_Buffer *B, const char *s);
// TODO: void luaL_addvalue (luaL_Buffer *B);
// TODO: lua_CFunction lua_tocfunction (lua_State *L, int index);
// TODO: void lua_setallocf (lua_State *L, lua_Alloc f, void *ud);
// TODO: void lua_register (lua_State *L, const char *name, lua_CFunction f);
// TODO: int lua_pcallk (lua_State *L, int nargs, int nresults, int msgh, lua_KContext ctx, lua_KFunction k);
// TODO: lua_Alloc lua_getallocf (lua_State *L, void **ud);
// TODO: void *lua_getextraspace (lua_State *L);
// TODO: lua_CFunction lua_atpanic (lua_State *L, lua_CFunction panicf); (?)
// TODO: int lua_load (lua_State *L, lua_Reader reader, void *data, const char *chunkname, const char *mode);
// TODO: int lua_yieldk (lua_State *L, int nresults, lua_KContext ctx, lua_KFunction k);
// TODO: lua_Hook lua_gethook (lua_State *L);
// TODO: int lua_getinfo (lua_State *L, const char *what, lua_Debug *ar);
// TODO: const char *lua_getlocal (lua_State *L, const lua_Debug *ar, int n);
// TODO: int lua_getstack (lua_State *L, int level, lua_Debug *ar);
// TODO: void lua_sethook (lua_State *L, lua_Hook f, int mask, int count);
// TODO: const char *lua_setlocal (lua_State *L, const lua_Debug *ar, int n);

// TODO: const char *lua_pushfstring (lua_State *L, const char *fmt, ...);
// TODO: const char *lua_pushvfstring (lua_State *L, const char *fmt, va_list argp);

LUAMOD_API int luaopen_multilua(lua_State* L) {
	static const struct luaL_Reg multilua [] = {
		{"new", multilua_new},
		{"current", multilua_current},
		{"close", multilua_close},
		{"openlibs", multilua_openlibs},
		{"absindex", multilua_absindex},
		{"arith", multilua_arith},
		{"call", multilua_call},
		{"checkstack", multilua_checkstack},
		{"compare", multilua_compare},
		{"concat", multilua_concat},
		{"copy", multilua_copy},
		{"createtable", multilua_createtable},
		{"error", multilua_error},
		{"gc", multilua_gc},
		{"getfield", multilua_getfield},
		{"luaversion", multilua_luaversion},
		{"xmove", multilua_xmove},
		{"yield", multilua_yield},
		{"getglobal", multilua_getglobal},
		{"geti", multilua_geti},
		{"getmetatable", multilua_getmetatable},
		{"gettable", multilua_gettable},
		{"gettop", multilua_gettop},
		{"getuservalue", multilua_getuservalue},
		{"insert", multilua_insert},
		{"isboolean", multilua_isboolean},
		{"iscfunction", multilua_iscfunction},
		{"isfunction", multilua_isfunction},
		{"isinteger", multilua_isinteger},
		{"islightuserdata", multilua_islightuserdata},
		{"isnil", multilua_isnil},
		{"isnone", multilua_isnone},
		{"isnoneornil", multilua_isnoneornil},
		{"isnumber", multilua_isnumber},
		{"isstring", multilua_isstring},
		{"istable", multilua_istable},
		{"isthread", multilua_isthread},
		{"isuserdata", multilua_isuserdata},
		{"isyieldable", multilua_isyieldable},
		{"len", multilua_len},
		{"newtable", multilua_newtable},
		{"newthread", multilua_newthread},
		{"newuserdata", multilua_newuserdata},
		{"next", multilua_next},
		{"pcall", multilua_pcall},
		{"pop", multilua_pop},
		{"pushboolean", multilua_pushboolean},
		{"pushglobaltable", multilua_pushglobaltable},
		{"pushinteger", multilua_pushinteger},
		{"pushstring", multilua_pushstring},
		{"pushnstring", multilua_pushnstring},
		{"pushnil", multilua_pushnil},
		{"pushnumber", multilua_pushnumber},
		{"pushvalue", multilua_pushvalue},
		{"pushlightuserdata", multilua_pushlightuserdata},
		{"rawequal", multilua_rawequal},
		{"rawget", multilua_rawget},
		{"rawgeti", multilua_rawgeti},
		{"rawlen", multilua_rawlen},
		{"rawset", multilua_rawset},
		{"rawseti", multilua_rawseti},
		{"rawsetp", multilua_rawsetp},
		{NULL, NULL},
	};

	luaL_newlib(L, multilua);

	// Set the library version
	lua_pushstring(L, MULTILUA_LIB);
	lua_setfield(L, -2, "version");

	return 1;
}
