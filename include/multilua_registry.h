#ifndef MULTILUA_REGISTRY
#define MULTILUA_REGISTRY

static const struct luaL_Reg multilua [] = {
	{"dump", multilua_dumpstack},
	{"new", multilua_new},
	{"close", multilua_manual_close},
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
	{"pushfloat", multilua_pushnumber},
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
	{"tofloat", multilua_tonumber},
	{"tofloatx", multilua_tonumberx},
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
	{"register", multilua_register},
	{"atpanic", multilua_atpanic},
	{"load", multilua_load},
	{"getallocf", multilua_getallocf},
	{"setallocf", multilua_setallocf},
	{"getextraspace", multilua_getextraspace},
	{"pcallk", multilua_pcallk},
	{"yieldk", multilua_yieldk},
	{"gethook", multilua_gethook},
	{"sethook", multilua_sethook},
	{"checkoption", multilua_checkoption},
	{"fetchable", multilua_fetchable},
	{"maxinteger", multilua_maxinteger},
	{"mininteger", multilua_mininteger},
	{"registryindex", multilua_registryindex},
	{"ridx_mainthread", multilua_ridx_mainthread},
	{"ridx_globals", multilua_ridx_globals},
	{"openbase", multilua_openbase},
	{"openpackage", multilua_openpackage},
	{"opencoroutine", multilua_opencoroutine},
	{"opentable", multilua_opentable},
	{"openio", multilua_openio},
	{"openos", multilua_openos},
	{"openstring", multilua_openstring},
	{"openmath", multilua_openmath},
	{"openutf8", multilua_openutf8},
	{"opendebug", multilua_opendebug},
	{"openbit32", multilua_openbit32},
	{"bitlib", multilua_bitlib},
	{NULL, NULL},
};

#endif
