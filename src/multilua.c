#include <multilua.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LUA_TYPE_HERE(_L) lua_typename(_L, lua_type(_L, -1))

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
			lua_pushstring(L, multilua[position].name);
			lua_pushcfunction(L, multilua[position].func);
			lua_rawset(L, -3);
		}

		position++;
	}
}

static int multilua_close(lua_State* L) {
	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		
		if(current_state != L) {
			lua_close(current_state);
			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_manual_close(lua_State* L) {
	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		
		if(current_state != L) {
			lua_close(current_state);
			
			lua_pushnil(L);
			lua_setfield(L, 1, "self");

			lua_pushboolean(L, true);
			return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pairs(lua_State* L) {
	lua_checkstack(L, 5);

	lua_getglobal(L, "pairs");
	lua_getmetatable(L, 1);
	lua_call(L, 1, 3);
	return 3;
}

static int multilua_index(lua_State* L) {
	if(lua_isinteger(L, 2)) {
		// Return stack type

		// Get the state...
		lua_getmetatable(L, 1);
		lua_pushstring(L, "self");
		lua_rawget(L, -2);

		if(lua_islightuserdata(L, -1)) {
			lua_State* current_state = lua_touserdata(L, -1);

			int t = lua_type(current_state, lua_tointeger(L, 2));
			lua_pushstring(L, lua_typename(L, t));
			return 1;
		}

		lua_pushnil(L);
		return 1;
	} else {
		// Return table key from metatable
		lua_getmetatable(L, 1);
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);
		return 1;
	}
}

static int multilua_new(lua_State* L) {
	lua_checkstack(L, 7);

	// Pushes the base object onto the stack
	// Lua will consistently GC this object.
	void* base = lua_newuserdata(L, 1);

	// Create the metatable for the self value:
	lua_newtable(L);

	util_installfuncs(L);

	// Create the finaliser
	lua_pushstring(L, "__gc");
	lua_pushcfunction(L, multilua_close);
	lua_rawset(L, -3);

	// Set indexing to the metatable
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, multilua_index);
	lua_rawset(L, -3);

	// Fix "pairs"
	lua_pushstring(L, "__pairs");
	lua_pushcfunction(L, multilua_pairs);
	lua_rawset(L, -3);

	// Supply a name for tostring:
	lua_pushstring(L, "__name");
	lua_pushstring(L, "multilua");
	lua_rawset(L, -3);

	// Get stack by #:
	lua_pushstring(L, "__len");
	lua_pushcfunction(L, multilua_gettop);
	lua_rawset(L, -3);

	// Pushing to stack by index method:
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, multilua_newindex);
	lua_rawset(L, -3);

	// Allow grabbing by call:
	lua_pushstring(L, "__call");
	lua_pushcfunction(L, multilua_metameth_call);
	lua_rawset(L, -3);

	// Create the self object...
	lua_State* new_state = luaL_newstate();

	// Memory allocation failure:
	if(!new_state) {
		lua_pushnil(L);
		return 1;
	}

	// Expose the state pointer:
	lua_pushstring(L, "self");
	lua_pushlightuserdata(L, new_state);
	lua_rawset(L, -3);

	// Install the metatable
	lua_setmetatable(L, -2);

	return 1;
}

static int multilua_dumpstack(lua_State* L) {
	lua_checkstack(L, lua_gettop(L) + 2);
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
	lua_checkstack(L, lua_gettop(L) + 2);

	lua_getfield(L, 1, "self");

	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		// Assumption: Ten standard libraries...
		lua_checkstack(current_state, 10 * 2);

		luaL_openlibs(current_state);
		lua_pushboolean(L, true);
		return 1;
	}
	// Don't need obj.self anymore:
	lua_pop(L, 1);

	// To not break return semantics:
	lua_pushnil(L);
	return 1;
}

static int multilua_metameth_call(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, lua_gettop(L) + 3);

	int bool_key = false;
	lua_Integer key = lua_tointegerx(L, 2, &bool_key);
	if(!key) {
		return luaL_error(L, "Expected an integer index.");
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		// Placeholder types for the switch:
		lua_Number x = 0;
		lua_Integer y = 0;
		lua_CFunction func = NULL;
		size_t string_length = 0;
		const char* string;
		void* ud = NULL;

		// Convert to a positive num...
		key = lua_absindex(current_state, key);
		
		int last_valid = lua_absindex(current_state, -1);
		if(key > last_valid) {
			return luaL_error(L, "Invalid stack index: %d", key);
		}

		// Find the right push function:
		int t = lua_type(current_state, key);
		switch(t) {
			case LUA_TNONE:
				// Index is still valid, so push something.
				lua_pushnil(L);
				return 1;
			case LUA_TNIL:
				lua_pushnil(L);
				return 1;
			case LUA_TNUMBER:
				if(lua_isinteger(current_state, key)) {
					y = lua_tointeger(current_state, key);
					lua_pushinteger(L, y);
				} else {
					x = lua_tonumber(current_state, key);
					lua_pushnumber(L, x);
				}
				return 1;
			case LUA_TBOOLEAN:
				y = lua_toboolean(current_state, key);
				lua_pushboolean(L, y);
				return 1;
			case LUA_TSTRING:
				string = lua_tolstring(current_state, key, &string_length);
				lua_pushlstring(L, string, string_length);
				return 1;
			case LUA_TTABLE:
				lua_pushvalue(current_state, key);
				lua_xmove(current_state, L, 1);
				return 1;
			case LUA_TFUNCTION:
				if(lua_iscfunction(current_state, key)) {
					func = lua_tocfunction(current_state, key);
					lua_pushcfunction(L, func);
					return 1;
				} else {
					lua_pushvalue(current_state, key);
					lua_xmove(current_state, L, 1);
					return 1;
				}
			case LUA_TUSERDATA:
				lua_pushvalue(current_state, key);
				lua_xmove(current_state, L, 1);
				return 1;
			case LUA_TTHREAD:
				lua_pushvalue(current_state, key);
				lua_xmove(current_state, L, 1);
				return 1;
			case LUA_TLIGHTUSERDATA:
				ud = lua_touserdata(current_state, key);
				lua_pushlightuserdata(L, ud);
				return 1;
			default:
				lua_pushvalue(current_state, key);
				lua_xmove(current_state, L, 1);
				return 1;
		}
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_newindex(lua_State* L) {
	// 1 - multilua state
	// 2 - key
	// 3 - value
	lua_checkstack(L, lua_gettop(L) + 3);

	int bool_key = false;
	lua_Integer key = lua_tointegerx(L, 2, &bool_key);
	if(!bool_key) {
		// Not a integer key, fallback to "regular" behaviour.
		
		lua_getmetatable(L, 1);
		lua_pushvalue(L, 2);
		lua_pushvalue(L, 3);
		lua_rawset(L, -3);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		// Placeholder types for the switch:
		lua_Number x = 0;
		lua_Integer y = 0;
		lua_CFunction func = NULL;
		size_t string_length = 0;
		const char* string;
		void* ud = NULL;

		// Convert to a positive num...
		key = lua_absindex(current_state, key);
		// Ensure space...
		lua_checkstack(current_state, key);

		// Where our pushed value will end up:
		size_t fromidx = lua_absindex(current_state, -1) + 1;

		// Pushing too high on the stack (invalid toidx):
		if(key > fromidx) {
			return luaL_error(L, "Pushing to invalid stack index: %d\n", key);
		}

		// Find the right push function:
		int t = lua_type(L, 3);
		switch(t) {
			case LUA_TNIL:
				lua_pushnil(current_state);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				break;
			case LUA_TNUMBER:
				if(lua_isinteger(L, 3)) {
					y = lua_tointeger(L, 3);
					lua_pushinteger(current_state, y);
				} else {
					x = lua_tonumber(L, 3);
					lua_pushnumber(current_state, x);
				}
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				break;
			case LUA_TBOOLEAN:
				x = lua_toboolean(L, 3);
				lua_pushboolean(current_state, x);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				break;
			case LUA_TSTRING:
				string = lua_tolstring(L, 3, &string_length);
				lua_pushlstring(current_state, string, string_length);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				break;
			case LUA_TTABLE:
				lua_pushvalue(L, 3);
				lua_xmove(L, current_state, 1);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				return 1;
			case LUA_TFUNCTION:
				if(lua_iscfunction(L, 3)) {
					func = lua_touserdata(L, 3);
					lua_pushcfunction(current_state, func);
					if(fromidx != key) {
						lua_copy(current_state, fromidx, key);
						lua_pop(current_state, fromidx);
					}
				} else {
					lua_pushvalue(L, 3);
					lua_xmove(L, current_state, 1);
					if(fromidx != key) {
						lua_copy(current_state, fromidx, key);
						lua_pop(current_state, fromidx);
					}
					return 1;
				}
				break;
			case LUA_TUSERDATA:
				lua_pushvalue(L, 3);
				lua_xmove(L, current_state, 1);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				return 1;
			case LUA_TTHREAD:
				lua_pushvalue(L, 3);
				lua_xmove(L, current_state, 1);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				return 1;
			case LUA_TLIGHTUSERDATA:
				ud = lua_touserdata(L, 3);
				lua_pushlightuserdata(current_state, ud);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				break;
			default:
				lua_pushvalue(L, 3);
				lua_xmove(L, current_state, 1);
				if(fromidx != key) {
					lua_copy(current_state, fromidx, key);
					lua_pop(current_state, fromidx);
				}
				return 1;
		}

		return 0;
	}

	luaL_error(L, "Error during assignment.");
	return 0;
}

static int multilua_fetchable(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, lua_gettop(L) + 3);

	int bool_key = false;
	lua_Integer key;

	if(lua_isnoneornil(L, 2)) {
		key = -1;
	} else {
		key = lua_tointegerx(L, 2, &bool_key);
		if(!key) {
			lua_pushnil(L);
			return 1;
		}
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		// Convert to a positive num...
		key = lua_absindex(current_state, key);
		
		int last_valid = lua_absindex(current_state, -1);
		if(key > last_valid) {
			lua_pushnil(L);
			return 1;
		}

		// Find the right push function:
		int t = lua_type(current_state, key);
		switch(t) {
			case LUA_TNONE:
				lua_pushboolean(L, true);
				return 1;
			case LUA_TNIL:
				lua_pushboolean(L, true);
				return 1;
			case LUA_TNUMBER:
				lua_pushboolean(L, true);
				return 1;
			case LUA_TBOOLEAN:
				lua_pushboolean(L, true);
				return 1;
			case LUA_TSTRING:
				lua_pushboolean(L, true);
				return 1;
			case LUA_TTABLE:
				lua_pushboolean(L, false);
				lua_pushstring(L, "table");
				return 2;
			case LUA_TFUNCTION:
				if(lua_iscfunction(current_state, key)) {
					lua_pushboolean(L, true);
					return 1;
				} else {
					lua_pushboolean(L, false);
					lua_pushstring(L, "function");
					return 2;
				}
			case LUA_TUSERDATA:
				lua_pushboolean(L, false);
				lua_pushstring(L, "userdata");
				return 2;
			case LUA_TTHREAD:
				lua_pushboolean(L, false);
				lua_pushstring(L, "thread");
				return 2;
			case LUA_TLIGHTUSERDATA:
				lua_pushboolean(L, true);
				return 1;
			default:
				lua_pushboolean(L, false);
				lua_pushstring(L, "unknown");
				return 2;
		}

		lua_pushnil(L);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_absindex(lua_State* L) {
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
				if(length == 2 && op[1] == '|') {
					lua_arith(current_state, LUA_OPBXOR);
				} else
				{
					lua_arith(current_state, LUA_OPBNOT);
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
	lua_checkstack(L, lua_gettop(L) + 2);

	int nargs_bool = false;
	int nargs = lua_tointegerx(L, 2, &nargs_bool);

	if(!nargs_bool) {
		lua_pushnil(L);
		return 1;
	}

	int nresults_bool = false;
	int nresults;

	if(lua_isnoneornil(L, 3)) {
		nresults = LUA_MULTRET;
	} else {
		nresults = lua_tointegerx(L, 3, &nresults_bool);
		if(!nresults_bool) {
			nresults = LUA_MULTRET;
		}
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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 3);

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

	lua_checkstack(L, lua_gettop(L) * 2);

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

		fromidx = lua_absindex(current_state, fromidx);
		toidx_bool = lua_absindex(current_state, toidx);

		lua_checkstack(current_state, toidx + 2);

		// Prevent a segfault.
		// Note: Lua doesn't provide this safety for some
		// fucking reason.
		if(fromidx == toidx) {
			lua_pushboolean(L, true);
			return 1;
		}

		// Copy the indicies...
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

	lua_checkstack(L, lua_gettop(L) + 3);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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

		int type = lua_getuservalue(current_state, index);
		lua_pushstring(L, lua_typename(L, type));
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_insert(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

	int nargs_bool = false;
	int nargs = lua_tointegerx(L, 2, &nargs_bool);
	if(!nargs_bool) {
		lua_pushnil(L);
		return 1;
	}

	int nresults_bool = false;
	int nresults;

	if(lua_isnoneornil(L, 3)) {
		nresults = LUA_MULTRET;
	} else {
		nresults = lua_tointegerx(L, 3, &nresults_bool);
		if(!nresults_bool) {
			nresults = LUA_MULTRET;
		}
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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

	int bool_index = false;
	lua_Integer index = lua_tointegerx(L, 2, &bool_index);
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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 3);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 3);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 3);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

	int bool_stat = false;
	int stat = lua_tointegerx(L, 2, &bool_stat);
	if(!bool_stat) {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 5);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
		lua_checkstack(current_state, 5);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) * 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);
	lua_pushinteger(L, LUA_REFNIL);
	return 1;
}

static int multilua_noref(lua_State* L) {
	lua_checkstack(L, lua_gettop(L) + 2);
	lua_pushinteger(L, LUA_NOREF);
	return 1;
}

static int multilua_registryindex(lua_State* L) {
	lua_checkstack(L, lua_gettop(L) + 2);
	lua_pushinteger(L, LUA_REGISTRYINDEX);
	return 1;
}

static int multilua_ridx_mainthread(lua_State* L) {
	lua_checkstack(L, lua_gettop(L) + 2);
	lua_pushinteger(L, LUA_RIDX_MAINTHREAD);
	return 1;
}

static int multilua_ridx_globals(lua_State* L) {
	lua_checkstack(L, lua_gettop(L) + 2);
	lua_pushinteger(L, LUA_RIDX_GLOBALS);
	return 1;
}

static int multilua_lsetmetatable(lua_State* L) {
	// 1 - multilua state
	// 2 - table_name
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
			lua_pushcfunction(L, r);
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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

	lua_CFunction func = NULL;
	if(lua_iscfunction(L, 2)) {
		func = lua_tocfunction(L, 2);
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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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
	lua_checkstack(L, lua_gettop(L) + 2);

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

static int multilua_register(lua_State* L) {
	// 1 - multilua state
	// 2 - name
	// 3 - function
	lua_checkstack(L, lua_gettop(L) + 2);

	const char* name = lua_tostring(L, 2);
	if(!name) {
		lua_pushnil(L);
		return 1;
	}

	lua_CFunction func = NULL;
	if(lua_iscfunction(L, 3)) {
		func = lua_touserdata(L, 3);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 6);

		lua_register(current_state, name, func);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_atpanic(lua_State* L) {
	// 1 - multilua state
	// 2 - panicf
	lua_checkstack(L, lua_gettop(L) + 2);

	lua_CFunction func = NULL;
	if(lua_iscfunction(L, 2)) {
		func = lua_touserdata(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_CFunction oldpanic = lua_atpanic(current_state, func);
		lua_pushcfunction(current_state, oldpanic);
		lua_pushcfunction(L, oldpanic);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_load(lua_State* L) {
	// 1 - multilua state
	// 2 - lua_Reader reader
	// 3 - data
	// 4 - chunkname
	// 5 - mode
	lua_checkstack(L, lua_gettop(L) + 2);

	lua_Reader reader = NULL;
	if(lua_isuserdata(L, 2)) {
		reader = lua_touserdata(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	void* data = NULL;
	if(lua_isuserdata(L, 3)) {
		data = lua_touserdata(L, 3);
	} else {
		lua_pushnil(L);
		return 1;
	}

	const char* chunkname = lua_tostring(L, 4);
	if(!chunkname) {
		chunkname = "=(multilua_load)";
	}

	const char* mode = lua_tostring(L, 5);
	if(!mode) {
		mode = "bt";
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 7);

		int r = lua_load(current_state, reader, data, chunkname, mode);
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
				lua_pushstring(L, "unknown");
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getallocf(lua_State * L) {
	// 1 - multilua state
	// 2 - **ud (optional)
	lua_checkstack(L, lua_gettop(L) + 2);

	void** ud = NULL;
	if(lua_islightuserdata(L, 2)) {
		ud = lua_touserdata(L, 2);
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_Alloc r = lua_getallocf(current_state, ud);

		lua_pushlightuserdata(current_state, r);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_setallocf(lua_State *L) {
	// 1 - multilua state
	// 2 - allocfunc
	// 3 - *ud
	lua_checkstack(L, lua_gettop(L) + 2);

	lua_Alloc* func = NULL;
	if(lua_islightuserdata(L, 2)) {
		func = lua_touserdata(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	void* ud = NULL;
	if(lua_islightuserdata(L, 3)) {
		ud = lua_touserdata(L, 3);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 4);

		lua_setallocf(current_state, *func, ud);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_getextraspace(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, lua_gettop(L) + 2);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 2);

		void* extra = lua_getextraspace(current_state);
		lua_pushlightuserdata(current_state, extra);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_pcallk(lua_State* L) {
	// 1 - multilua state
	// 2 - int nargs
	// 3 - int nresults
	// 4 - int msgh
	// 5 - lua_KContext ctx (numeric)
	// 6 - lua_KFunction* k (pointer)
	lua_checkstack(L, lua_gettop(L) + 4);

	int bool_nargs = false;
	int nargs = lua_tointegerx(L, 2, &bool_nargs);
	if(!bool_nargs) {
		lua_pushnil(L);
		return 1;
	}

	int bool_nresults = false;
	int nresults = lua_tointegerx(L, 3, &bool_nresults);
	if(!bool_nresults) {
		lua_pushnil(L);
		return 1;
	}

	int bool_msgh = false;
	int msgh = lua_tointegerx(L, 4, &bool_msgh);
	if(!bool_msgh) {
		lua_pushnil(L);
		return 1;
	}

	int bool_ctx = false;
	lua_KContext ctx = lua_tointegerx(L, 5, &bool_ctx);
	if(!bool_ctx) {
		lua_pushnil(L);
		return 1;
	}

	lua_KFunction* func = NULL;
	if(lua_islightuserdata(L, 6)) {
		func = lua_touserdata(L, 6);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 7);

		int r = lua_pcallk(current_state, nargs, nresults, msgh, ctx, *func);
		switch(r) {
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
				lua_pushstring(L, "unknown");
				break;
		}
		return 2;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_yieldk(lua_State* L) {
	// 1 - multilua state
	// 2 - int nresults
	// 3 - lua_KContext ctx (numeric)
	// 4 - lua_KFunction* k
	lua_checkstack(L, lua_gettop(L) + 2);

	int bool_nresults = false;
	int nresults = lua_tointegerx(L, 2, &bool_nresults);
	if(!bool_nresults) {
		lua_pushnil(L);
		return 1;
	}

	int bool_ctx = false;
	lua_KContext ctx = lua_tointegerx(L, 3, &bool_ctx);
	if(!bool_ctx) {
		lua_pushnil(L);
		return 1;
	}

	lua_KFunction* k = NULL;
	if(lua_islightuserdata(L, 4)) {
		k = lua_touserdata(L, 4);
	} else {
		lua_pushnil(L);
		return 1;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, LUA_MINSTACK + 6);

		lua_yieldk(current_state, nresults, ctx, *k);
		lua_pushboolean(L, true);
		return 1;
	}	

	lua_pushnil(L);
	return 1;
}

static int multilua_gethook(lua_State* L) {
	// 1 - multilua state
	lua_checkstack(L, lua_gettop(L) + 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_Hook hook = lua_gethook(current_state);
		lua_pushlightuserdata(current_state, hook);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_sethook(lua_State *L) {
	// 1 - multilua state
	// 2 - hook (pointer)
	// 3 - mask (truthy table)
	// 4 - int count (default: 0)
	lua_checkstack(L, lua_gettop(L) + 4);

	int t;

	lua_Hook func = NULL;
	if(lua_islightuserdata(L, 2)) {
		func = lua_touserdata(L, 2);
	} else {
		lua_pushnil(L);
		return 1;
	}

	// Get the mask:

	int mask = 0;
	t = lua_type(L, 3);
	if(t != LUA_TTABLE) {
		lua_pushnil(L);
		return 1;
	}

	t = lua_getfield(L, 3, "call");
	if(t != LUA_TNIL) {
		mask |= LUA_MASKCALL;
	}
	lua_pop(L, -1);

	t = lua_getfield(L, 3, "return");
	if(t != LUA_TNIL) {
		mask |= LUA_MASKRET;
	}
	lua_pop(L, -1);

	t = lua_getfield(L, 3, "line");
	if(t != LUA_TNIL) {
		mask |= LUA_MASKLINE;
	}
	lua_pop(L, -1);

	t = lua_getfield(L, 3, "count");
	if(t != LUA_TNIL) {
		mask |= LUA_MASKCOUNT;
	}
	lua_pop(L, -1);

	// Count defaults to 0.
	int bool_count = false;
	int count = lua_tointegerx(L, 4, &bool_count);
	if(!bool_count) {
		count = 0;
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 6);

		lua_sethook(current_state, func, mask, count);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_checkoption(lua_State* L) {
	// 1 - multilua state
	// 2 - int arg_index
	// 3 - def (can be NULL)
	// 4 - lst (table array)
	lua_checkstack(L, lua_gettop(L) + 2);

	int bool_arg = false;
	int arg = lua_tointegerx(L, 2, &bool_arg);
	if(!bool_arg) {
		lua_pushnil(L);
		return 1;
	}

	// Can be NULL:
	const char* def = lua_tostring(L, 3);

	int t = lua_type(L, 4);
	if(t != LUA_TTABLE) {
		lua_pushnil(L);
		return 1;
	}
	lua_len(L, 4);
	lua_Integer tlength = lua_tointeger(L, -1);
	lua_pop(L, -1);

	lua_checkstack(L, tlength);

	const char** lst = calloc(sizeof(const char*), tlength + 2);
	if(!lst) {
		lua_pushnil(L);
		return 1;
	}

	for(size_t i = 0; i < tlength; i++) {
		t = lua_geti(L, 4, i);
		if(t == LUA_TNIL) {
			break;
		} else
		if(t != LUA_TSTRING) {
			free(lst);
			lua_pushnil(L);
			return 1;
		}

		lst[i] = lua_tostring(L, -1);
	}

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);

		lua_Integer r = luaL_checkoption(current_state, arg, def, lst);
		free(lst);
		lua_pushinteger(current_state, r);
		lua_pushboolean(L, true);
		return 1;
	}

	free(lst);
	lua_pushnil(L);
	return 1;
}

static int multilua_maxinteger(lua_State* L) {
	lua_pushinteger(L, LUA_MAXINTEGER);
	return 1;
}

static int multilua_mininteger(lua_State* L) {
	lua_pushinteger(L, LUA_MININTEGER);
	return 1;
}

static int multilua_openbase(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_base);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_openpackage(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_package);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_opencoroutine(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_coroutine);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_opentable(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_table);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_openio(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_io);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_openos(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_os);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_openstring(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_string);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_openmath(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_math);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_openutf8(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_utf8);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_opendebug(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		lua_pushcfunction(current_state, luaopen_debug);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_openbit32(lua_State* L) {
	lua_checkstack(L, 3);

	lua_getfield(L, 1, "self");
	if(lua_islightuserdata(L, -1)) {
		lua_State* current_state = lua_touserdata(L, -1);
		lua_checkstack(current_state, 3);

		#ifdef LUA_COMPAT_BITLIB
		lua_pushcfunction(current_state, luaopen_bit32);
		#else
		lua_pushnil(current_state);
		#endif
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_bitlib(lua_State* L) {
	lua_checkstack(L, 3);

	#ifdef LUA_COMPAT_BITLIB
	lua_pushboolean(L, true);
	#else
	lua_pushboolean(L, false);
	#endif
	
	return 1;
}

static int multilua_version_major(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_VERSION_MAJOR);
	return 1;
}

static int multilua_version_minor(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_VERSION_MINOR);
	return 1;
}

static int multilua_version_release(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_VERSION_RELEASE);
	return 1;
}

static int multilua_lua_version_string(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_VERSION);
	return 1;
}

static int multilua_release_string(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_RELEASE);
	return 1;
}

static int multilua_luacopyright(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_COPYRIGHT);
	return 1;
}

static int multilua_luaauthors(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_AUTHORS);
	return 1;
}

static int multilua_signature(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushstring(L, LUA_SIGNATURE);
	return 1;
}

static int multilua_multret(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_MULTRET);
	return 1;
}

static int multilua_minstack(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_MINSTACK);
	return 1;
}

static int multilua_ridx_last(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_RIDX_LAST);
	return 1;
}

static int multilua_user_header(lua_State* L) {
	lua_checkstack(L, 3);

	#ifdef LUA_USER_H
	lua_pushstring(L, LUA_USER_H);
	#else
	lua_pushnil(L);
	#endif

	return 1;
}

static int multilua_upvalueindex(lua_State* L) {
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
		lua_checkstack(current_state, 3);

		size_t r = lua_upvalueindex(index);
		lua_pushinteger(current_state, r);

		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_hookcall(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_HOOKCALL);
	return 1;
}

static int multilua_hookreturn(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_HOOKRET);
	return 1;
}

static int multilua_hookline(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_HOOKLINE);
	return 1;
}

static int multilua_hookcount(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_HOOKCOUNT);
	return 1;
}

static int multilua_hooktailcall(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_HOOKTAILCALL);
	return 1;
}

static int multilua_maskcall(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_MASKCALL);
	return 1;
}

static int multilua_maskreturn(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_MASKRET);
	return 1;
}

static int multilua_maskline(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_MASKLINE);
	return 1;
}

static int multilua_maskcount(lua_State* L) {
	lua_checkstack(L, 3);

	lua_pushinteger(L, LUA_MASKCOUNT);
	return 1;
}

// Lua 5.2 API compat...
#ifndef LUA_COMPAT_APIINTCASTS
#define lua_pushunsigned(L,n)   lua_pushinteger(L, (lua_Integer)(n))
#define lua_tounsignedx(L,i,is) ((lua_Unsigned)lua_tointegerx(L,i,is))
#define lua_tounsigned(L,i)     lua_tounsignedx(L,(i),NULL)
#endif

static int multilua_pushunsigned(lua_State* L) {
	// 1 - multilua state
	// 2 - integer

	lua_checkstack(L, lua_gettop(L) + 2);

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

		lua_pushunsigned(current_state, n);
		lua_pushboolean(L, true);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

static int multilua_tounsignedx(lua_State* L) {
	// 1 - multilua state
	// 2 - integer

	lua_checkstack(L, lua_gettop(L) + 2);

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
		lua_Integer r = lua_tounsignedx(current_state, index, &success);

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

static int multilua_tounsigned(lua_State* L) {
	// 1 - multilua state
	// 2 - index
	lua_checkstack(L, lua_gettop(L) + 2);

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

		lua_Integer r = lua_tounsigned(current_state, index);
		lua_pushinteger(L, r);
		return 1;
	}

	lua_pushnil(L);
	return 1;
}

// TODO: Should we expose sizeof values for all the Lua types?

LUAMOD_API int luaopen_multilua(lua_State* L) {
	luaL_newlib(L, multilua);

	// Set the library version
	lua_pushstring(L, MULTILUA_LIB);
	lua_setfield(L, -2, "version");

	// Calling the library table should call new:
	lua_pushcfunction(L, multilua_new);
	lua_setfield(L, -2, "__call");

	// Set the library to it's own metatable
	lua_pushvalue(L, -1);
	lua_setmetatable(L, -2);

	return 1;
}
