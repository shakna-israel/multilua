#ifndef MULTILUA_LIB
#define MULTILUA_LIB "0.0.0"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

static int multilua_new(lua_State* L);
static int multilua_close(lua_State* L);
static int multilua_openlibs(lua_State* L);
static int multilua_absindex(lua_State* L);
static int multilua_current(lua_State* L);
static int multilua_arith(lua_State* L);
static int multilua_call(lua_State* L);
static int multilua_checkstack(lua_State* L);
static int multilua_compare(lua_State* L);
static int multilua_concat(lua_State* L);

#endif
