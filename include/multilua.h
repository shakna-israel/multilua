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
static int multilua_copy(lua_State* L);
static int multilua_createtable(lua_State* L);
static int multilua_error(lua_State* L);
static int multilua_gc(lua_State* L);
static int multilua_getfield(lua_State* L);
static int multilua_luaversion(lua_State* L);
static int multilua_xmove(lua_State* L);
static int multilua_yield(lua_State* L);
static int multilua_getglobal(lua_State* L);
static int multilua_geti(lua_State* L);
static int multilua_getmetatable(lua_State* L);
static int multilua_gettable(lua_State* L);
static int multilua_gettop(lua_State* L);
static int multilua_getuservalue(lua_State* L);
static int multilua_insert(lua_State* L);
static int multilua_isboolean(lua_State* L);
static int multilua_iscfunction(lua_State* L);
static int multilua_isfunction(lua_State* L);
static int multilua_isinteger(lua_State* L);
static int multilua_islightuserdata(lua_State* L);
static int multilua_isnil(lua_State* L);
static int multilua_isnone(lua_State* L);
static int multilua_isnoneornil(lua_State* L);
static int multilua_isnumber(lua_State* L);
static int multilua_isstring(lua_State* L);
static int multilua_istable(lua_State* L);
static int multilua_isthread(lua_State* L);
static int multilua_isuserdata(lua_State* L);
static int multilua_isyieldable(lua_State* L);
static int multilua_len(lua_State* L);
static int multilua_newtable(lua_State* L);
static int multilua_equal(lua_State* L);
static int multilua_newthread(lua_State* L);
static int multilua_newuserdata(lua_State* L);
static int multilua_next(lua_State* L);
static int multilua_pcall(lua_State* L);
static int multilua_pop(lua_State* L);
static int multilua_pushboolean(lua_State* L);
static int multilua_pushglobaltable(lua_State* L);
static int multilua_pushinteger(lua_State* L);
static int multilua_pushnstring(lua_State* L);
static int multilua_pushstring(lua_State* L);
static int multilua_pushnil(lua_State* L);
static int multilua_pushnumber(lua_State* L);
static int multilua_pushvalue(lua_State* L);
static int multilua_pushlightuserdata(lua_State* L);
static int multilua_rawequal(lua_State* L);
static int multilua_rawget(lua_State* L);
static int multilua_rawgeti(lua_State* L);
static int multilua_rawlen(lua_State* L);
static int multilua_rawset(lua_State* L);
static int multilua_rawseti(lua_State* L);
static int multilua_rawsetp(lua_State* L);
static int multilua_remove(lua_State* L);
static int multilua_replace(lua_State* L);
static int multilua_rotate(lua_State* L);
static int multilua_setfield(lua_State* L);
static int multilua_setglobal(lua_State* L);
static int multilua_seti(lua_State* L);
static int multilua_setmetatable(lua_State* L);
static int multilua_settable(lua_State* L);
static int multilua_settop(lua_State* L);
static int multilua_setuservalue(lua_State* L);
static int multilua_status(lua_State* L);
static int multilua_toboolean(lua_State* L);
static int multilua_tointeger(lua_State* L);
static int multilua_tointegerx(lua_State* L);
static int multilua_tostring(lua_State* L);
static int multilua_tonumber(lua_State* L);
static int multilua_tonumberx(lua_State* L);
static int multilua_topointer(lua_State* L);


#endif
