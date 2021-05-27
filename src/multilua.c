#include <stdbool.h>
#include <string.h>

#include <multilua.h>

#define LUA_TYPE_HERE(_L) lua_typename(_L, lua_type(_L, -1))

static const struct luaL_Reg multilua [] = {
	{"dump", multilua_dumpstack},
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
	{"remove", multilua_remove},
	{"replace", multilua_replace},
	{"rotate", multilua_rotate},
	{"setfield", multilua_setfield},
	{"setglobal", multilua_setglobal},
	{"seti", multilua_seti},
	{"setmetatable", multilua_setmetatable},
	{"settable", multilua_settable},
	{"settop", multilua_settop},
	{"setuservalue", multilua_setuservalue},
	{"status", multilua_status},
	{"toboolean", multilua_toboolean},
	{"tointeger", multilua_tointeger},
	{"tointegerx", multilua_tointegerx},
	{"tostring", multilua_tostring},
	{"tonumber", multilua_tonumber},
	{"tonumberx", multilua_tonumberx},
	{"topointer", multilua_topointer},
	{"tothread", multilua_tothread},
	{"touserdata", multilua_touserdata},
	{"type", multilua_type},
	{"gethookcount", multilua_gethookcount},
	{"gethookmask", multilua_gethookmask},
	{"getupvalue", multilua_getupvalue},
	{"setupvalue", multilua_setupvalue},
	{"upvalueid", multilua_upvalueid},
	{"upvaluejoin", multilua_upvaluejoin},
	{"argcheck", multilua_argcheck},
	{"argerror", multilua_argerror},
	{"callmeta", multilua_callmeta},
	{"checkany", multilua_checkany},
	{"checkinteger", multilua_checkinteger},
	{"checkstring", multilua_checkstring},
	{"checknumber", multilua_checknumber},
	{"lcheckstack", multilua_lcheckstack},
	{"checktype", multilua_checktype},
	{"checkudata", multilua_checkudata},
	{"checkversion", multilua_checkversion},
	{"dofile", multilua_dofile},
	{"dostring", multilua_dostring},
	{"execresult", multilua_execresult},
	{"fileresult", multilua_fileresult},
	{"getmetafield", multilua_getmetafield},
	{"lgetmetatable", multilua_lgetmetatable},
	{"getsubtable", multilua_getsubtable},
	{"gsub", multilua_gsub},
	{"llen", multilua_llen},
	{"loadbuffer", multilua_loadbuffer},
	{"loadbufferx", multilua_loadbufferx},
	{"loadfile", multilua_loadfile},
	{"loadfilex", multilua_loadfilex},
	{"loadstring", multilua_loadstring},
	{"newmetatable", multilua_newmetatable},
	{"optinteger", multilua_optinteger},
	{"optstring", multilua_optstring},
	{"optnumber", multilua_optnumber},
	{"ref", multilua_ref},
	{"refnil", multilua_refnil},
	{"noref", multilua_noref},
	{"lsetmetatable", multilua_lsetmetatable},
	{"testudata", multilua_testudata},
	{"ltostring", multilua_ltostring},
	{"traceback", multilua_traceback},
	{"unref", multilua_unref},
	{"where", multilua_where},
	{"resume", multilua_resume},
	{"rawgetp", multilua_rawgetp},
	{"pushthread", multilua_pushthread},
	{"tocfunction", multilua_tocfunction},
	{"pushcclosure", multilua_pushcclosure},
	{"pushcfunction", multilua_pushcfunction},
	{"newreg", multilua_newreg},
	{"tabletoreg", multilua_tabletoreg},
	{"setfuncs", multilua_setfuncs},
	{"requiref", multilua_requiref},
	{"buffinit", multilua_buffinit},
	{"buffinitsize", multilua_buffinitsize},
	{"prepbuffsize", multilua_prepbuffsize},
	{"prepbuffer", multilua_prepbuffer},
	{"addchar", multilua_addchar},
	{"addstring", multilua_addstring},
	{"addsize", multilua_addsize},
	{"addvalue", multilua_addvalue},
	{"pushresult", multilua_pushresult},
	{"pushresultsize", multilua_pushresultsize},
	{"newlib", multilua_newlib},
	{NULL, NULL},
};

void util_installfuncs(lua_State* L) {
	size_t position = 0;
	while(true) {
		lua_checkstack(L, 3);

		if(multilua[position].name == NULL) {
			break;
		} else
		if(multilua[position].func == NULL) {
			break;
		} else {
			lua_pushcfunction(L, multilua[position].func);
			lua_setfield(L, -2, multilua[position].name);
		}

		position++;
	}
}

void util_installmeta(lua_State* L) {
	lua_checkstack(L, 6);

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

	lua_checkstack(L, 2);

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
	lua_checkstack(L, 6);

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
	lua_checkstack(L, 8);

	lua_State* new_state = luaL_newstate();

	// Memory allocation failure:
	if(!new_state) {
		lua_pushnil(L);
		return 1;
	}

	lua_checkstack(new_state, 2);

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
	lua_checkstack(L, 3);

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

static int multilua_dumpstack(lua_State* L) {
	lua_checkstack(L, 3);
	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		int i = lua_gettop(current_state);
		while(i) {
			int t = lua_type(current_state, i);
			switch(t) {
				case LUA_TSTRING:
					printf("%d:`%s'\n", i, lua_tostring(current_state, i));
					break;
				case LUA_TBOOLEAN:
					printf("%d: %s\n",i,lua_toboolean(current_state, i) ? "true" : "false");
					break;
				case LUA_TNUMBER:
					printf("%d: %g\n",  i, lua_tonumber(current_state, i));
					break;
				default:
					printf("%d: %s\n", i, lua_typename(current_state, t));
					break;
			}
			i--;
		}

		lua_pushboolean(L, true);
		return 1;
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

static int multilua_openlibs(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		// Assumption: Ten standard libraries...
		lua_checkstack(current_state, 10 * 2);

		luaL_openlibs(current_state);
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

static int multilua_absindex(lua_State* L) {
	lua_checkstack(L, 4);

	int idx = lua_tointeger(L, 2);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_checkstack(current_state, 2);
		
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
	lua_checkstack(L, 4);

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
		lua_checkstack(current_state, 4);
		
		switch(op[0]) {
			case '+':
				lua_arith(current_state, LUA_OPADD);
				break;
			case '-':
				if(length == 2 && op[1] == 'u') {
					lua_arith(current_state, LUA_OPUNM);
				} else {
					lua_arith(current_state, LUA_OPSUB);
				}
				break;
			case '*':
				lua_arith(current_state, LUA_OPMUL);
				break;
			case '/':
				if(length == 2 && op[1] == '/') {
					lua_arith(current_state, LUA_OPIDIV);
				} else {
					lua_arith(current_state, LUA_OPDIV);
				}
				break;
			case '%':
				lua_arith(current_state, LUA_OPMOD);
				break;
			case '^':
				lua_arith(current_state, LUA_OPPOW);
				break;
			case '&':
				lua_arith(current_state, LUA_OPBAND);
				break;
			case '|':
				lua_arith(current_state, LUA_OPBOR);
				break;
			case '<':
				lua_arith(current_state, LUA_OPSHL);
				break;
			case '>':
				lua_arith(current_state, LUA_OPSHR);
				break;
			case '~':
				if(length == 2 && op[1] == '!') {
					lua_arith(current_state, LUA_OPBNOT);
				} else
				if(length == 2 && op[1] == '|') {
					lua_arith(current_state, LUA_OPBXOR);
				} else
				{
					lua_pushnil(L);
					return 1;
				}
				break;
			default:
				lua_pushnil(L);
				return 1;
		}

		if(lua_isinteger(current_state, -1)) {
			lua_Integer y = lua_tointeger(current_state, -1);
			lua_pushinteger(L, y);
			return 1;
		} else
		if(lua_isnumber(current_state, -1)) {
			lua_Number x = lua_tonumber(current_state, -1);
			lua_pushnumber(L, x);
			return 1;
		} else
		{
			lua_pushnil(L);
			return 1;
		}
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

static int multilua_call(lua_State* L) {
	lua_checkstack(L, 3);

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
		lua_checkstack(current_state, nargs + nresults + 2);
		lua_call(current_state, nargs, nresults);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checkstack(lua_State* L) {
	lua_checkstack(L, 3);

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
	lua_checkstack(L, 6);

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
		lua_checkstack(current_state, 2);

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
	lua_checkstack(L, 3);

	int n_bool = false;
	int n = lua_tointegerx(L, 2, &n_bool);

	if(!n_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(L, n + 2);

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

	lua_checkstack(L, 6);

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

	lua_checkstack(L, 3);

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
		lua_checkstack(current_state, narr + nrec + 2);

		lua_createtable(current_state, narr, nrec);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_error(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 2);

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
	lua_checkstack(L, 3);

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
		lua_checkstack(current_state, 6);

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
	lua_checkstack(L, 3);

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
		lua_checkstack(current_state, 2);

		int type = lua_getfield(current_state, index, k);

		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_luaversion(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* from_state = lua_touserdata(L, -1);
		lua_checkstack(from_state, 3);

		lua_getfield(L, 2, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* to_state = lua_touserdata(L, -1);
			
			int n_bool = false;
			int n = lua_tointegerx(L, 3, &n_bool);
			if(!n_bool) {
				lua_pushnil(L);
				return 1;
			}
			lua_checkstack(to_state, n + 2);

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
	lua_checkstack(L, 3);

	int nresults_bool = false;
	int nresults = lua_tointegerx(L, 2, &nresults_bool);

	if(!nresults_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, nresults + 3);

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
	lua_checkstack(L, 3);

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
	lua_checkstack(L, 3);

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
		lua_checkstack(current_state, 2);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		int type = lua_gettable(current_state, index);

		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_gettop(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);
	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int index_bool = false;
	int index = lua_tointegerx(L, 2, &index_bool);

	if(!index_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_len(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newtable(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_newtable(current_state);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newthread(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 6);

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
	lua_checkstack(L, 3);

	int bool_size = false;
	size_t size = lua_tointegerx(L, 2, &bool_size);
	if(!bool_size) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

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
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

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
	lua_checkstack(L, 6);

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
		lua_checkstack(current_state, nargs + nresults + 2);

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
	lua_checkstack(L, 4);

	int bool_n = false;
	int n = lua_tointegerx(L, 2, &bool_n);
	if(!n) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, n + 2);

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
	lua_checkstack(L, 4);

	int b = lua_toboolean(L, 2);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_pushboolean(current_state, b);
		lua_pushboolean(L, true);
		return 1;
	}


	lua_pushnil(L);
	return 1;
}

static int multilua_pushglobaltable(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

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
	lua_checkstack(L, 4);

	int n_bool = false;
	lua_Integer n = lua_tointegerx(L, 2, &n_bool);
	if(!n_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

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
	lua_checkstack(L, 4);

	size_t length = 0;
	const char* s = lua_tolstring(L, 2, &length);
	if(!s) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

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
	lua_checkstack(L, 5);

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
		lua_checkstack(current_state, 3);

		lua_pushlstring(current_state, s, length);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushnil(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int n_bool = false;
	lua_Number n = lua_tonumberx(L, 2, &n_bool);
	if(!n_bool) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	if(!lua_islightuserdata(L, 2)) {
		lua_pushnil(L);
		return 1;
	}

	void* ptr = lua_touserdata(L, 2);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 5);

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
		lua_checkstack(current_state, 5);

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
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 5);

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
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, -2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

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
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

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
	lua_checkstack(L, 3);

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
		lua_checkstack(current_state, 5);

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
	lua_checkstack(L, 5);

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
			lua_checkstack(current_state, 5);
			
			lua_rawsetp(current_state, index, ptr);

			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_remove(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 5);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_remove(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_replace(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_replace(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rotate(lua_State* L) {
	// 1 - multilua state
	// 2 - idx
	// 3 - n
	lua_checkstack(L, 3);

	int bool_idx = false;
	int idx = lua_tointegerx(L, 2, &bool_idx);
	if(!bool_idx) {
		lua_pushnil(L);
		return 1;
	}

	int bool_n = false;
	int n = lua_tointegerx(L, 3, &bool_n);
	if(!bool_n) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, n + 2);

		lua_rotate(current_state, idx, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_setfield(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - key
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	size_t length = 0;
	const char* key = lua_tolstring(L, 3, &length);
	if(!key) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_setfield(current_state, index, key);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_setglobal(lua_State* L) {
	// 1 - multilua state
	// 2 - name
	lua_checkstack(L, 3);

	const char* name = lua_tostring(L, 2);
	if(!name) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_setglobal(current_state, name);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_seti(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - n
	lua_checkstack(L, 3);

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
		lua_checkstack(current_state, n + 2);

		lua_seti(current_state, index, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_setmetatable(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_setmetatable(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_settable(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_settable(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_settop(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_settop(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_setuservalue(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_setuservalue(current_state, index);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_status(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		int r = lua_status(current_state);
		switch(r) {
			case LUA_OK:
				lua_pushstring(L, "ok");
				break;
			case LUA_YIELD:
				lua_pushstring(L, "yield");
				break;
			case LUA_ERRRUN:
				lua_pushstring(L, "runtime");
				break;
			case LUA_ERRMEM:
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRERR:
				lua_pushstring(L, "error");
				break;
			case LUA_ERRGCMM:
				lua_pushstring(L, "gcmeta");
				break;
			default:
				lua_pushstring(L, "other");
				break;
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_toboolean(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		int x = lua_toboolean(current_state, index);
		lua_pushboolean(L, x);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tointeger(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_Integer r = lua_tointeger(current_state, index);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tointegerx(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		int success = false;
		lua_Integer r = lua_tointegerx(current_state, index, &success);

		if(!success) {
			lua_pushnil(L);
			return 1;
		} else {
			lua_pushinteger(L, r);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tostring(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		size_t length = 0;
		const char* s = lua_tolstring(current_state, index, &length);
		if(!s) {
			lua_pushnil(L);
			return 1;
		} else {
			lua_pushlstring(L, s, length);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tonumber(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_Number r = lua_tonumber(current_state, index);
		lua_pushnumber(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tonumberx(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		int success = false;
		lua_Number r = lua_tonumberx(current_state, index, &success);
		if(!success) {
			lua_pushnil(L);
			return 1;
		} else {
			lua_pushnumber(L, r);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_topointer(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		void* ptr = (void*)lua_topointer(current_state, index);
		if(ptr == NULL) {
			lua_pushnil(L);
			return 1;
		} else {
			lua_pushlightuserdata(L, ptr);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tothread(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_State* thread = lua_tothread(current_state, index);
		if(thread == NULL) {
			lua_pushnil(L);
			return 1;
		} else {
			lua_pushlightuserdata(L, thread);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_touserdata(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 3);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		void* ptr = (void*)lua_touserdata(current_state, index);
		if(ptr == NULL) {
			lua_pushnil(L);
			return 1;
		} else {
			lua_pushlightuserdata(L, ptr);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_type(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushstring(L, lua_typename(L, lua_type(current_state, index)));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_gethookcount(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		int r = lua_gethookcount(current_state);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_gethookmask(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		int r = lua_gethookmask(current_state);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getupvalue(lua_State* L) {
	// 1 - multilua state
	// 2 - funcindex
	// 3 - n
	lua_checkstack(L, 5);

	int bool_funcindex = false;
	int funcindex = lua_tointegerx(L, 2, &bool_funcindex);
	if(!bool_funcindex) {
		lua_pushnil(L);
		return 1;
	}

	int bool_n = false;
	int n = lua_tointegerx(L, 3, &bool_n);
	if(!bool_n) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, n + 3);

		const char* r = lua_getupvalue(current_state, funcindex, n);
		if(r == NULL) {
			lua_pushnil(L);
			return 1;
		} else
		if(r[0] == '\0') {
			lua_pushboolean(L, true);
			return 1;
		} else {
			lua_pushstring(L, r);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_setupvalue(lua_State* L) {
	// 1 - multilua state
	// 2 - funcindex
	// 3 - n
	lua_checkstack(L, 5);

	int bool_funcindex = false;
	int funcindex = lua_tointegerx(L, 2, &bool_funcindex);
	if(!bool_funcindex) {
		lua_pushnil(L);
		return 1;
	}

	int bool_n = false;
	int n = lua_tointegerx(L, 3, &bool_n);
	if(!bool_n) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		const char* r = lua_setupvalue(current_state, funcindex, n);
		if(r == NULL) {
			lua_pushnil(L);
			return 1;
		} else
		if(r[0] == '\0') {
			lua_pushboolean(L, true);
			return 1;
		} else
		{
			lua_pushstring(L, r);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_upvalueid(lua_State* L) {
	// 1 - multilua state
	// 2 - funcindex
	// 3 - n
	lua_checkstack(L, 5);

	int bool_funcindex = false;
	int funcindex = lua_tointegerx(L, 2, &bool_funcindex);
	if(!bool_funcindex) {
		lua_pushnil(L);
		return 1;
	}

	int bool_n = false;
	int n = lua_tointegerx(L, 3, &bool_n);
	if(!bool_n) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		void* ptr = lua_upvalueid(current_state, funcindex, n);
		lua_pushlightuserdata(L, ptr);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_upvaluejoin(lua_State* L) {
	// 1 - multilua state
	// 2 - funcindexA
	// 3 - nA
	// 4 - funcindexB
	// 5 - nB
	lua_checkstack(L, 7);

	int bool_funcindexA = false;
	int funcindexA = lua_tointegerx(L, 2, &bool_funcindexA);
	if(!bool_funcindexA) {
		lua_pushnil(L);
		return 1;
	}

	int bool_nA = false;
	int nA = lua_tointegerx(L, 3, &bool_nA);
	if(!bool_nA) {
		lua_pushnil(L);
		return 1;
	}

	int bool_funcindexB = false;
	int funcindexB = lua_tointegerx(L, 4, &bool_funcindexB);
	if(!bool_funcindexB) {
		lua_pushnil(L);
		return 1;
	}

	int bool_nB = false;
	int nB = lua_tointegerx(L, 5, &bool_nB);
	if(!bool_nB) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, nA + nB + 4);

		lua_upvaluejoin(current_state, funcindexA, nA, funcindexB, nB);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_argcheck(lua_State* L) {
	// 1 - multilua state
	// 2 - bool cond
	// 3 - arg
	// 4 - extramsg
	lua_checkstack(L, 6);

	int cond = lua_toboolean(L, 2);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 3, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	const char* extramsg = lua_tostring(L, 4);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 5);

		luaL_argcheck(current_state, cond, arg, extramsg);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_argerror(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - extramsg
	lua_checkstack(L, 5);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	const char* extramsg = lua_tostring(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 5);

		luaL_argerror(current_state, arg, extramsg);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_callmeta(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - field
	lua_checkstack(L, 5);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	const char* field = lua_tostring(L, 3);
	if(!field) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		int r = luaL_callmeta(current_state, index, field);
		lua_pushboolean(L, r);
		return 1;
	}	

	lua_pushnil(L);
	return 1;
}

static int multilua_checkany(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	lua_checkstack(L, 4);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		luaL_checkany(current_state, arg);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checkinteger(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	lua_checkstack(L, 4);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		lua_Integer r = luaL_checkinteger(current_state, arg);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checkstring(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	lua_checkstack(L, 4);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		size_t length = 0;
		const char* s = luaL_checklstring(current_state, arg, &length);
		if(s == NULL) {
			lua_pushnil(L);
		} else {
			lua_pushlstring(L, s, length);
		}

		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checknumber(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	lua_checkstack(L, 4);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		lua_Number r = luaL_checknumber(current_state, arg);
		lua_pushnumber(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_lcheckstack(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - msg
	lua_checkstack(L, 5);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	const char* msg = lua_tostring(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		luaL_checkstack(current_state, arg, msg);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checktype(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - type
	lua_checkstack(L, 3);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	int type = LUA_TNONE;
	const char* typename = lua_tostring(L, 3);

	if(!typename) {
		lua_pushnil(L);
		return 1;
	} else
	if(strcmp("none", typename) == 0) {
		type = LUA_TNONE;
	} else
	if(strcmp("nil", typename) == 0) {
		type = LUA_TNIL;
	} else
	if(strcmp("number", typename) == 0) {
		type = LUA_TNUMBER;
	} else
	if(strcmp("boolean", typename) == 0) {
		type = LUA_TBOOLEAN;
	} else
	if(strcmp("string", typename) == 0) {
		type = LUA_TSTRING;
	} else
	if(strcmp("table", typename) == 0) {
		type = LUA_TTABLE;
	} else
	if(strcmp("function", typename) == 0) {
		type = LUA_TFUNCTION;
	} else
	if(strcmp("userdata", typename) == 0) {
		type = LUA_TUSERDATA;
	} else
	if(strcmp("thread", typename) == 0) {
		type = LUA_TTHREAD;
	} else
	if(strcmp("lightuserdata", typename) == 0) {
		type = LUA_TLIGHTUSERDATA;
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		luaL_checktype(current_state, arg, type);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checkudata(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - tname
	lua_checkstack(L, 5);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	const char* typename = lua_tostring(L, 3);
	if(!typename) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 4);

		void* ptr = (void*)luaL_checkudata(current_state, arg, typename);
		lua_pushlightuserdata(L, ptr);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checkversion(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		luaL_checkversion(current_state);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_dofile(lua_State* L) {
	// 1 - multilua state
	// 2 - filename
	lua_checkstack(L, 3);

	const char* filename = lua_tostring(L, 2);
	if(!filename) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 5);

		if(luaL_dofile(current_state, filename)) {
			// Errors!
			lua_pushboolean(L, false);
		} else {
			// No errors.
			lua_pushboolean(L, true);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_dostring(lua_State* L) {
	// 1 - multilua state
	// 2 - string
	lua_checkstack(L, 4);

	const char* source = lua_tostring(L, 2);
	if(!source) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 5);

		if(luaL_dostring(current_state, source)) {
			// Errors
			lua_pushboolean(L, false);
		} else {
			// No errors.
			lua_pushboolean(L, true);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_execresult(lua_State* L) {
	// 1 - multilua state
	// 2 - stat
	lua_checkstack(L, 3);

	int bool_stat = false;
	int stat = lua_tointegerx(L, 2, &bool_stat);
	if(!bool_stat) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 5);

		luaL_execresult(current_state, stat);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_fileresult(lua_State* L) {
	// 1 - multilua state
	// 2 - stat
	// 3 - filename
	lua_checkstack(L, 5);

	int bool_stat = false;
	int stat = lua_tointegerx(L, 2, &bool_stat);
	if(!bool_stat) {
		lua_pushnil(L);
		return 1;
	}

	const char* filename = lua_tostring(L, 3);
	if(!filename) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 5);

		luaL_fileresult(current_state, stat, filename);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getmetafield(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - field
	lua_checkstack(L, 5);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	const char* field = lua_tostring(L, 3);
	if(!field) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		int type = luaL_getmetafield(current_state, index, field);
		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_lgetmetatable(lua_State* L) {
	// 1 - multilua state
	// 2 - tname
	lua_checkstack(L, 4);

	const char* table_name = lua_tostring(L, 2);
	if(!table_name) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		int type = luaL_getmetatable(current_state, table_name);
		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getsubtable(lua_State* L) {
	// 1 - multilua state
	// 2 - idx
	// 3 - fname
	lua_checkstack(L, 5);

	int bool_idx = false;
	int idx = lua_tointegerx(L, 2, &bool_idx);
	if(!bool_idx) {
		lua_pushnil(L);
		return 1;
	}

	const char* fname = lua_tostring(L, 3);
	if(!fname) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 7);

		int r = luaL_getsubtable(current_state, idx, fname);
		lua_pushboolean(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_gsub(lua_State* L) {
	// 1 - multilua state
	// 2 - source
	// 3 - pattern
	// 4 - replacement
	lua_checkstack(L, 6);

	const char* source = lua_tostring(L, 2);
	if(!source) {
		lua_pushnil(L);
		return 1;
	}

	const char* pattern = lua_tostring(L, 3);
	if(!pattern) {
		lua_pushnil(L);
		return 1;
	}

	const char* replacement = lua_tostring(L, 4);
	if(!replacement) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 7);

		const char* r = luaL_gsub(current_state, source, pattern, replacement);
		lua_pushstring(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_llen(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_Integer r = luaL_len(current_state, index);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_loadbuffer(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	// 3 - name
	lua_checkstack(L, 5);

	size_t length = 0;
	const char* buffer = lua_tolstring(L, 2, &length);
	if(!buffer) {
		lua_pushnil(L);
		return 1;
	}

	const char* chunkname = lua_tostring(L, 3);
	if(!chunkname) {
		chunkname = "=(loadbuffer)";
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 5);

		int r = luaL_loadbuffer(current_state, buffer, length, chunkname);
		switch(r) {
			case LUA_OK:
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
				break;
			case LUA_ERRSYNTAX:
				lua_pushboolean(L, false);
				lua_pushstring(L, "syntax");
				break;
			case LUA_ERRMEM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRGCMM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "gcmeta");
				break;
			default:
				lua_pushboolean(L, false);
				lua_pushstring(L, "other");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_loadbufferx(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	// 3 - chunkname
	// 4 - mode
	lua_checkstack(L, 6);

	size_t length = 0;
	const char* buffer = lua_tolstring(L, 2, &length);
	if(!buffer) {
		lua_pushnil(L);
		return 1;
	}

	const char* chunkname = lua_tostring(L, 3);
	if(!chunkname) {
		chunkname = "=(loadbuffer)";
	}

	const char* mode = lua_tostring(L, 4);
	
	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 6);

		int r = luaL_loadbufferx(current_state, buffer, length, chunkname, mode);
		switch(r) {
			case LUA_OK:
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
				break;
			case LUA_ERRSYNTAX:
				lua_pushboolean(L, false);
				lua_pushstring(L, "syntax");
				break;
			case LUA_ERRMEM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRGCMM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "gcmeta");
				break;
			default:
				lua_pushboolean(L, false);
				lua_pushstring(L, "other");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_loadfile(lua_State* L) {
	// 1 - multilua state
	// 2 - filename
	lua_checkstack(L, 4);

	const char* filename = lua_tostring(L, 2);
	if(!filename) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 7);

		int r = luaL_loadfile(current_state, filename);
		switch(r) {
			case LUA_OK:
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
				break;
			case LUA_ERRSYNTAX:
				lua_pushboolean(L, false);
				lua_pushstring(L, "syntax");
				break;
			case LUA_ERRMEM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRGCMM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "gcmeta");
				break;
			case LUA_ERRFILE:
				lua_pushboolean(L, false);
				lua_pushstring(L, "file");
				break;
			default:
				lua_pushboolean(L, false);
				lua_pushstring(L, "other");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_loadfilex(lua_State* L) {
	// 1 - multilua state
	// 2 - filename
	// 3 - mode
	lua_checkstack(L, 5);

	const char* filename = lua_tostring(L, 2);
	if(!filename) {
		lua_pushnil(L);
		return 1;
	}

	const char* mode = lua_tostring(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 6);

		int r = luaL_loadfilex(current_state, filename, mode);
		switch(r) {
			case LUA_OK:
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
				break;
			case LUA_ERRSYNTAX:
				lua_pushboolean(L, false);
				lua_pushstring(L, "syntax");
				break;
			case LUA_ERRMEM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRGCMM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "gcmeta");
				break;
			case LUA_ERRFILE:
				lua_pushboolean(L, false);
				lua_pushstring(L, "file");
				break;
			default:
				lua_pushboolean(L, false);
				lua_pushstring(L, "other");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_loadstring(lua_State* L) {
	// 1 - multilua state
	// 2 - source
	lua_checkstack(L, 4);

	const char* source = lua_tostring(L, 2);
	if(!source) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 6);

		int r = luaL_loadstring(current_state, source);
		switch(r) {
			case LUA_OK:
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
				break;
			case LUA_ERRSYNTAX:
				lua_pushboolean(L, false);
				lua_pushstring(L, "syntax");
				break;
			case LUA_ERRMEM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRGCMM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "gcmeta");
				break;
			case LUA_ERRFILE:
				lua_pushboolean(L, false);
				lua_pushstring(L, "file");
				break;
			default:
				lua_pushboolean(L, false);
				lua_pushstring(L, "other");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newmetatable(lua_State* L) {
	// 1 - multilua state
	// 2 - table_name
	lua_checkstack(L, 4);

	const char* table_name = lua_tostring(L, 2);
	if(!table_name) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		int r = luaL_newmetatable(current_state, table_name);
		if(r == 0) {
			lua_pushboolean(L, true);
		} else {
			lua_pushboolean(L, false);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_optinteger(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - default
	lua_checkstack(L, 6);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	int bool_default = false;
	lua_Integer default_int = lua_tointegerx(L, 3, &bool_default);
	if(!bool_default) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		lua_Integer r = luaL_optinteger(current_state, arg, default_int);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_optstring(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - default
	lua_checkstack(L, 5);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	size_t length = 0;
	const char* default_string = lua_tolstring(L, 3, &length);
	if(!default_string) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 3);

		const char* r = luaL_optlstring(current_state, arg, default_string, &length);
		if(!r) {
			lua_pushnil(L);
		} else {
			lua_pushlstring(L, r, length);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_optnumber(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - default
	lua_checkstack(L, 5);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	int bool_default = false;
	lua_Integer default_int = lua_tointegerx(L, 3, &bool_default);
	if(!bool_default) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		lua_Number r = luaL_optnumber(current_state, arg, default_int);
		lua_pushnumber(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_ref(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		int r = luaL_ref(current_state, index);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_refnil(lua_State* L) {
	lua_checkstack(L, 2);
	lua_pushinteger(L, LUA_REFNIL);
	return 1;
}

static int multilua_noref(lua_State* L) {
	lua_checkstack(L, 2);
	lua_pushinteger(L, LUA_NOREF);
	return 1;
}

static int multilua_lsetmetatable(lua_State* L) {
	// 1 - multilua state
	// 2 - table_name
	lua_checkstack(L, 4);

	const char* table_name = lua_tostring(L, 2);
	if(!table_name) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 7);

		luaL_setmetatable(current_state, table_name);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_testudata(lua_State* L) {
	// 1 - multilua state
	// 2 - arg
	// 3 - table_name
	lua_checkstack(L, 5);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	const char* table_name = lua_tostring(L, 3);
	if(!table_name) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, arg + 2);

		void* ptr = (void*)luaL_testudata(current_state, arg, table_name);
		if(ptr == NULL) {
			lua_pushnil(L);
		} else {
			lua_pushlightuserdata(L, ptr);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_ltostring(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		size_t length = 0;
		const char* string = luaL_tolstring(current_state, index, &length);
		lua_pushlstring(L, string, length);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_traceback(lua_State* L) {
	// 1 - multilua state
	// 2 - msg
	// 3 - level
	lua_checkstack(L, 5);

	const char* msg = lua_tostring(L, 2);

	int bool_level = false;
	int level = lua_tointegerx(L, 3, &bool_level);
	if(!bool_level) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 5);

		luaL_traceback(L, current_state, msg, level);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_unref(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - reference
	lua_checkstack(L, 5);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	int bool_reference = false;
	int reference = lua_tointegerx(L, 3, &bool_reference);
	if(!bool_reference) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 6);

		luaL_unref(current_state, index, reference);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_where(lua_State* L) {
	// 1 - multilua state
	// 2 - level
	lua_checkstack(L, 4);

	int bool_level = false;
	int level = lua_tointegerx(L, 2, &bool_level);
	if(!bool_level) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		luaL_where(current_state, level);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_resume(lua_State* L) {
	// 1 - multilua state
	// 2 - thread
	// 3 - nargs
	lua_checkstack(L, 5);

	int bool_nargs = false;
	int nargs = lua_tointegerx(L, 3, &bool_nargs);
	if(!bool_nargs) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, nargs + 7);

		lua_State* thread = lua_touserdata(L, 2);
		int r = lua_resume(current_state, thread, nargs);
		switch(r) {
			case LUA_YIELD:
				lua_pushboolean(L, true);
				lua_pushstring(L, "yield");
				break;
			case LUA_OK:
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
				break;
			case LUA_ERRRUN:
				lua_pushboolean(L, false);
				lua_pushstring(L, "runtime");
				break;
			case LUA_ERRMEM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "memory");
				break;
			case LUA_ERRERR:
				lua_pushboolean(L, false);
				lua_pushstring(L, "error");
				break;
			case LUA_ERRGCMM:
				lua_pushboolean(L, false);
				lua_pushstring(L, "gcmeta");
				break;
			default:
				lua_pushboolean(L, false);
				lua_pushstring(L, "other");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_rawgetp(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - pointer
	lua_checkstack(L, 5);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	if(lua_islightuserdata(L, 3)) {
		void* ptr = (void*)lua_touserdata(L, 3);

		lua_getfield(L, 1, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* current_state = lua_touserdata(L, -1);
			lua_checkstack(current_state, 6);

			int type = lua_rawgetp(current_state, index, ptr);
			lua_pushstring(L, lua_typename(L, type));
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushthread(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		int r = lua_pushthread(current_state);
		lua_pushboolean(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tocfunction(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, 4);

	int bool_index = false;
	int index = lua_tointegerx(L, 2, &bool_index);
	if(!bool_index) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_CFunction r = lua_tocfunction(current_state, index);
		if(r == NULL) {
			lua_pushnil(L);
		} else {
			lua_pushlightuserdata(L, r);
		}
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushcclosure(lua_State* L) {
	// 1 - multilua state
	// 2 - cfunction
	// 3 - n
	lua_checkstack(L, 5);

	int bool_n = false;
	int n = lua_tointegerx(L, 3, &bool_n);
	if(!bool_n || n > 255 || n < 0) {
		lua_pushnil(L);
		return 1;
	}

	lua_CFunction func = NULL;
	if(lua_islightuserdata(L, 2)) {
		func = lua_touserdata(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, n + 3);

		lua_pushcclosure(current_state, func, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushcfunction(lua_State* L) {
	// 1 - multilua state
	// 2 - func
	lua_checkstack(L, 4);

	lua_CFunction func = NULL;
	if(lua_islightuserdata(L, 2)) {
		func = lua_touserdata(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

		lua_pushcfunction(current_state, func);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newreg(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(L, LUA_MINSTACK + 2);

		void* ptr = lua_newuserdata(current_state, sizeof(luaL_Reg));
		lua_pushlightuserdata(L, ptr);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tabletoreg(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	// 3 - table
	lua_checkstack(L, 5);

	int bool_tindex = false;
	int table_index = lua_tointegerx(L, 2, &bool_tindex);
	if(!bool_tindex) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		// Get table size:
		size_t table_length = 0;

		lua_pushnil(L);
		while(lua_next(L, table_index) != 0) {
			table_length++;

			// Remove value, keep key for iterating...
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		lua_checkstack(current_state, LUA_MINSTACK + (table_length * 2) + 2);

		// Allocate structures
		luaL_Reg** reg_array = lua_newuserdata(current_state, sizeof(luaL_Reg*) * (table_length + 2));
		for(size_t i = 0; i < table_length; i++) {
			reg_array[i] = lua_newuserdata(current_state, sizeof(luaL_Reg));
		}

		// Build array
		size_t i = 0;
		lua_pushnil(L);
		while(lua_next(L, table_index) != 0) {
			// -2 - key
			// -1 - value

			// Non-string keys will fuckup... But there should be no non-string keys...
			size_t length = 0;
			const char* key = lua_tolstring(L, -2, &length);

			lua_CFunction* func = lua_touserdata(L, -1);

			// Copy the key...
			char* key_copy = lua_newuserdata(current_state, sizeof(char) * length + 1);
			for(size_t idx = 0; idx < length; idx++) {
				key_copy[idx] = key[idx];
			}

			reg_array[i]->name = key_copy;
			reg_array[i]->func = *func;

			// Remove value, keep key for iterating...
			lua_pop(L, 1);
			i++;
		}

		// Install the sentinal
		reg_array[i]->name = NULL;
		reg_array[i]->func = NULL;

		// Return array
		lua_pushlightuserdata(current_state, reg_array);
		lua_pushlightuserdata(L, reg_array);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_setfuncs(lua_State* L) {
	// 1 - multilua state
	// 2 - lreg
	// 3 - nup
	lua_checkstack(L, 5);

	int bool_nup = false;
	int nup = lua_tointegerx(L, 3, &bool_nup);
	if(!bool_nup) {
		nup = 0;
	}

	if(lua_islightuserdata(L, 2)) {
		const luaL_Reg* lreg = lua_touserdata(L, 2);

		lua_getfield(L, 1, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* current_state = lua_touserdata(L, -1);
			lua_checkstack(current_state, nup + 4);

			luaL_setfuncs(current_state, lreg, nup);
			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newlib(lua_State* L) {
	// 1 - multilua state
	// 2 - reg pointer
	lua_checkstack(L, 4);

	if(lua_islightuserdata(L, 2)) {
		const luaL_Reg** lreg = lua_touserdata(L, 2);

		lua_getfield(L, 1, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* current_state = lua_touserdata(L, -1);

			size_t table_length = 0;
			while(true) {
				const luaL_Reg* item = lreg[table_length];
				if(item->name == NULL) {
					break;
				}
				if(item->func == NULL) {
					break;
				}

				table_length++;
			}
			lua_checkstack(current_state, (table_length * 2) + 2);

			lua_createtable(current_state, 0, table_length / sizeof((lreg)[0]) - 1);
			luaL_setfuncs(current_state, *lreg, 0);

			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_requiref(lua_State* L) {
	// 1 - multilua state
	// 2 - modname
	// 3 - openf
	// 4 - glb
	lua_checkstack(L, 6);

	const char* modname = lua_tostring(L, 2);
	if(!modname) {
		lua_pushnil(L);
		return 1;
	}

	lua_CFunction* openf = NULL;
	if(lua_islightuserdata(L, 3)) {
		openf = lua_touserdata(L, 3);
	} else {
		lua_pushnil(L);
		return 1;
	}

	int glb = lua_toboolean(L, 4);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 5);

		luaL_requiref(current_state, modname, *openf, glb);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_buffinit(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, sizeof(luaL_Buffer));

		luaL_Buffer* buff = lua_newuserdata(current_state, sizeof(luaL_Buffer));
		luaL_buffinit(current_state, buff);
		lua_pushlightuserdata(current_state, buff);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_buffinitsize(lua_State* L) {
	// 1 - multilua state
	// 2 - size
	lua_checkstack(L, 4);

	int bool_size = false;
	size_t size = lua_tointegerx(L, 2, &bool_size);
	if(!bool_size) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, sizeof(luaL_Buffer));

		luaL_Buffer* buff = lua_newuserdata(current_state, sizeof(luaL_Buffer));
		luaL_buffinitsize(current_state, buff, size);
		lua_pushlightuserdata(current_state, buff);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_prepbuffsize(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	// 3 - size
	lua_checkstack(L, 5);

	int bool_size = false;
	size_t size = lua_tointegerx(L, 3, &bool_size);
	if(!bool_size) {
		lua_pushnil(L);
		return 1;
	}

	luaL_Buffer* buff = NULL;
	if(lua_islightuserdata(L, 2)) {
		buff = lua_touserdata(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, sizeof(luaL_Buffer));

		char* ptr = luaL_prepbuffsize(buff, size);
		lua_pushlightuserdata(current_state, ptr);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_prepbuffer(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	lua_checkstack(L, 4);

	if(lua_islightuserdata(L, 2)) {
		luaL_Buffer* buff = lua_touserdata(L, 2);

		lua_getfield(L, 1, "self");
		if(lua_islightuserdata(L, -1)) {
			lua_State* current_state = lua_touserdata(L, -1);
			lua_checkstack(L, sizeof(luaL_Buffer));

			char* ptr = luaL_prepbuffer(buff);
			lua_pushlightuserdata(current_state, ptr);
			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_addchar(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	// 3 - character
	lua_checkstack(L, 5);

	const char* str = lua_tostring(L, 3);
	char c = str[0];

	if(lua_islightuserdata(L, 2)) {
		luaL_Buffer* buff = lua_touserdata(L, 2);

		luaL_addchar(buff, c);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_addstring(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	// 3 - string
	lua_checkstack(L, 5);

	size_t length = 0;
	const char* string = lua_tostring(L, 3);
	if(!string) {
		lua_pushnil(L);
		return 1;
	}

	if(lua_islightuserdata(L, 2)) {
		luaL_Buffer* buff = lua_touserdata(L, 2);

		luaL_addlstring(buff, string, length);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_addsize(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	// 3 - n
	lua_checkstack(L, 5);

	int bool_n = false;
	size_t n = lua_tointegerx(L, 3, &bool_n);
	if(!bool_n) {
		lua_pushnil(L);
		return 1;
	}

	if(lua_islightuserdata(L, 2)) {
		luaL_Buffer* buff = lua_touserdata(L, 2);

		luaL_addsize(buff, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_addvalue(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	lua_checkstack(L, 4);

	if(lua_islightuserdata(L, 2)) {
		luaL_Buffer* buff = lua_touserdata(L, 2);

		luaL_addvalue(buff);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushresult(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	lua_checkstack(L, 4);

	if(lua_islightuserdata(L, 2)) {
		luaL_Buffer* buff = lua_touserdata(L, 2);

		luaL_pushresult(buff);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pushresultsize(lua_State* L) {
	// 1 - multilua state
	// 2 - buffer
	// 3 - size
	lua_checkstack(L, 5);

	int bool_size = false;
	size_t size = lua_tointegerx(L, 3, &bool_size);
	if(!bool_size) {
		lua_pushnil(L);
		return 1;
	}

	if(lua_islightuserdata(L, 2)) {
		luaL_Buffer* buff = lua_touserdata(L, 2);

		luaL_pushresultsize(buff, size);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

// These are slightly harder to wrap:
// TODO: int luaL_checkoption (lua_State *L, int arg, const char *def, const char *const lst[]);
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

LUAMOD_API int luaopen_multilua(lua_State* L) {
	luaL_newlib(L, multilua);

	// Set the library version
	lua_pushstring(L, MULTILUA_LIB);
	lua_setfield(L, -2, "version");

	return 1;
}
