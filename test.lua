local multilua = require "multilua"

-- Test: We got a library
assert(multilua ~= nil)
assert(type(multilua) == 'table')

-- Test: We have a library version
assert(multilua.version ~= nil)

-- Test: creation
do
	assert(type(multilua.new) == 'function')
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	-- Test: closing
	assert(type(multilua.close) == 'function')
	assert(type(multilua.close(obj)) == 'nil')
	assert(obj.self == nil)
end

-- Test: Multiple closes
-- If this fails, you'll get a double-free!
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.close(obj)) == 'nil')
	assert(obj.self == nil)
	assert(type(multilua.close(obj)) == 'nil')
	assert(obj.self == nil)
end

-- Test: metaclosing
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')
	assert(obj:close() == nil)
end

-- Test multiple states can exist
-- simultaneously
do
	local obj1 = multilua.new()
	local obj2 = multilua.new()

	assert(obj1.self ~= obj2.self)

	obj1:close()
	obj2:close()
end

-- Test equivalents
do
	local obj1 = multilua.new()
	local obj2 = multilua.new()

	assert(obj1 == obj1)
	assert(obj1 ~= obj2)

	obj1:close()
	obj2:close()
end


-- Test openlibs
do
	assert(type(multilua.openlibs) == 'function')

	local obj = multilua.new()
	multilua.openlibs(obj)

	-- String Library now exists:
	multilua.getglobal(obj, "string")
	t = multilua.type(obj, -1)
	assert(t == 'table')
end

-- Test: openlibs meta
do
	assert(type(multilua.openlibs) == 'function')

	local obj = multilua.new()
	assert(type(obj.openlibs) == 'function')
	obj:openlibs()

	-- String Library now exists:
	obj:getglobal("string")
	t = obj:type(-1)
	assert(t == 'table')
end

-- Test: gettop
do
	assert(type(multilua.gettop) == 'function')

	local obj = multilua.new()

	for i = 1, 10 do
		multilua.pushinteger(obj, i)
	end

	assert(multilua.gettop(obj) == 10)
end

-- Test: gettop meta
do
	assert(type(multilua.gettop) == 'function')

	local obj = multilua.new()
	assert(type(obj.gettop) == 'function')

	for i = 1, 10 do
		obj:pushinteger(i)
	end

	assert(obj:gettop() == 10)
end

-- Test: #
do
	assert(type(multilua.gettop) == 'function')

	local obj = multilua.new()

	for i = 1, 10 do
		multilua.pushinteger(obj, i)
	end

	assert(#obj == 10)
end

-- Test: absindex
do
	assert(type(multilua.absindex) == 'function')

	local obj = multilua.new()

	-- Nothing on stack, should be 0.
	assert(multilua.absindex(obj, -1) == 0)

	multilua.pushinteger(obj, 1)

	-- Number on stack, should be 0.
	assert(multilua.absindex(obj, -1) == 1)
end

-- Test: absindex meta
do
	assert(type(multilua.absindex) == 'function')

	local obj = multilua.new()

	-- Nothing on stack, should be 0.
	assert(obj:absindex(-1) == 0)

	obj:pushinteger(1)

	-- Number on stack, should be 0.
	assert(obj:absindex(-1) == 1)
end

-- Test: arith
do
	assert(type(multilua.arith) == 'function')

	local obj = multilua.new()

	-- Addition
	multilua.pushinteger(obj, 1)
	multilua.pushinteger(obj, 1)
	assert(multilua.arith(obj, "+") == 2.0)

	-- Negation
	multilua.pushinteger(obj, 1)
	multilua.pushinteger(obj, 1)
	assert(multilua.arith(obj, "-") == 0.0)

	-- Unary Minus
	multilua.pushinteger(obj, 1)
	assert(multilua.arith(obj, "-u") == -1.0)

	-- Multiply
	multilua.pushinteger(obj, 1)
	multilua.pushinteger(obj, 2)
	assert(multilua.arith(obj, "*") == 2.0)

	-- Divide
	multilua.pushinteger(obj, 2)
	multilua.pushinteger(obj, 1)
	assert(multilua.arith(obj, "/") == 2.0)

	-- Integer Divide
	multilua.pushinteger(obj, 2)
	multilua.pushinteger(obj, 1)
	assert(multilua.arith(obj, "//") == 2)

	-- Modulus
	multilua.pushinteger(obj, 2)
	multilua.pushinteger(obj, 2)
	assert(multilua.arith(obj, "%") == 0.0)

	-- Exponent
	multilua.pushinteger(obj, 2)
	multilua.pushinteger(obj, 2)
	assert(multilua.arith(obj, "^") == 4.0)

	-- Bitwise AND
	multilua.pushinteger(obj, 5)
	multilua.pushinteger(obj, 3)
	assert(multilua.arith(obj, "&") == 1.0)

	-- Bitwise OR
	multilua.pushinteger(obj, 5)
	multilua.pushinteger(obj, 3)
	assert(multilua.arith(obj, "|") == 7.0)

	-- Bitwise Leftshift
	multilua.pushinteger(obj, 5)
	multilua.pushinteger(obj, 3)
	assert(multilua.arith(obj, "<<") == 40.0)

	-- Bitwise Rightshift
	multilua.pushinteger(obj, 5)
	multilua.pushinteger(obj, 3)
	assert(multilua.arith(obj, ">>") == 0.0)

	-- Bitwise NOT
	multilua.pushinteger(obj, -2)
	assert(multilua.arith(obj, "~!") == 1.0)

	-- Bitwise XOR
	multilua.pushinteger(obj, 5)
	multilua.pushinteger(obj, 3)
	assert(multilua.arith(obj, "~|") == 6.0)
end

-- Test: arith meta
do
	assert(type(multilua.arith) == 'function')

	local obj = multilua.new()
	assert(type(obj.arith) == 'function')

	-- Addition
	obj:pushinteger(1)
	obj:pushinteger(1)
	assert(obj:arith("+") == 2.0)

	-- Negation
	obj:pushinteger(1)
	obj:pushinteger(1)
	assert(obj:arith("-") == 0.0)

	-- Unary Minus
	obj:pushinteger(1)
	assert(obj:arith("-u") == -1.0)

	-- Multiply
	obj:pushinteger(1)
	obj:pushinteger(2)
	assert(obj:arith("*") == 2.0)

	-- Divide
	obj:pushinteger(2)
	obj:pushinteger(1)
	assert(obj:arith("/") == 2.0)

	-- Integer Divide
	obj:pushinteger(2)
	obj:pushinteger(1)
	assert(obj:arith("//") == 2)

	-- Modulus
	obj:pushinteger(2)
	obj:pushinteger(2)
	assert(obj:arith("%") == 0.0)

	-- Exponent
	obj:pushinteger(2)
	obj:pushinteger(2)
	assert(obj:arith("^") == 4.0)

	-- Bitwise AND
	obj:pushinteger(5)
	obj:pushinteger(3)
	assert(obj:arith("&") == 1.0)

	-- Bitwise OR
	obj:pushinteger(5)
	obj:pushinteger(3)
	assert(obj:arith("|") == 7.0)

	-- Bitwise Leftshift
	obj:pushinteger(5)
	obj:pushinteger(3)
	assert(obj:arith("<<") == 40.0)

	-- Bitwise Rightshift
	obj:pushinteger(5)
	obj:pushinteger(3)
	assert(obj:arith(">>") == 0.0)

	-- Bitwise NOT
	obj:pushinteger(-2)
	assert(obj:arith("~!") == 1.0)

	-- Bitwise XOR
	obj:pushinteger(5)
	obj:pushinteger(3)
	assert(obj:arith("~|") == 6.0)
end

-- TODO: Test: call
do
	assert(type(multilua.call) == 'function')
end

-- TODO: Test: call meta
do
	assert(type(multilua.call) == 'function')
end

-- Test: checkstack
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.checkstack) == 'function')
	assert(multilua.checkstack(obj, 2) == true)

	obj:close()
end

-- Test: checkstack meta
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.checkstack) == 'function')
	assert(obj:checkstack(2) == true)

	obj:close()
end

-- Test: compare
do
	assert(type(multilua.compare) == 'function')

	local obj = multilua.new()

	multilua.pushinteger(obj, 10)
	multilua.pushinteger(obj, 11)

	assert(multilua.compare(obj, -1, -2, "==") == false)
	assert(multilua.compare(obj, -1, -2, "<") == false)
	assert(multilua.compare(obj, -1, -2, "<=") == false)

	assert(multilua.compare(obj, -2, -1, "==") == false)
	assert(multilua.compare(obj, -2, -1, "<") == true)
	assert(multilua.compare(obj, -2, -1, "<=") == true)
end

-- Test: compare meta
do
	assert(type(multilua.compare) == 'function')

	local obj = multilua.new()
	assert(type(obj.compare) == 'function')

	obj:pushinteger(10)
	obj:pushinteger(11)
	
	assert(obj:compare(-1, -2, "==") == false)
	assert(obj:compare(-1, -2, "<") == false)
	assert(obj:compare(-1, -2, "<=") == false)

	assert(obj:compare(-2, -1, "==") == false)
	assert(obj:compare(-2, -1, "<") == true)
	assert(obj:compare(-2, -1, "<=") == true)
end

-- Test: concat
do
	assert(type(multilua.concat) == 'function')

	local obj = multilua.new()

	multilua.pushstring(obj, "h")
	multilua.pushstring(obj, "e")
	multilua.pushstring(obj, "l")
	multilua.pushstring(obj, "l")
	multilua.pushstring(obj, "o")

	assert(multilua.concat(obj, 5))
	assert(multilua.tostring(obj, -1) == "hello")
end

-- Test: concat meta
do
	assert(type(multilua.concat) == 'function')

	local obj = multilua.new()
	assert(type(obj.concat) == 'function')

	obj:pushstring("h")
	obj:pushstring("e")
	obj:pushstring("l")
	obj:pushstring("l")
	obj:pushstring("o")

	assert(obj:concat(5))
	assert(obj:tostring(-1) == "hello")
end

-- Test: copy
do
	assert(type(multilua.copy) == 'function')

	local obj = multilua.new()

	multilua.pushinteger(obj, 3)

	assert(multilua.copy(obj, -1, -2))
	assert(multilua.tointeger(obj, -2) == 3)
end

-- Test: copy meta
do
	assert(type(multilua.copy) == 'function')

	local obj = multilua.new()
	assert(type(obj.copy) == 'function')

	obj:pushinteger(3)

	assert(obj:copy(-1, -2))
	assert(obj:tointeger(-2) == 3)
end

-- Test: createtable
do
	assert(type(multilua.createtable) == 'function')

	local obj = multilua.new()

	multilua.createtable(obj)
	assert(multilua.type(obj, -1) == 'table')

	multilua.createtable(obj, 10)
	assert(multilua.type(obj, -1) == 'table')

	multilua.createtable(obj, nil, 10)
	assert(multilua.type(obj, -1) == 'table')

	multilua.createtable(obj, 10, 10)
	assert(multilua.type(obj, -1) == 'table')
end

-- Test: createtable meta
do
	assert(type(multilua.createtable) == 'function')

	local obj = multilua.new()
	assert(type(obj.createtable) == 'function')

	obj:createtable()
	assert(obj:type(-1) == 'table')

	obj:createtable(10)
	assert(obj:type(-1) == 'table')

	obj:createtable(nil, 10)
	assert(obj:type(-1) == 'table')

	obj:createtable(10, 10)
	assert(obj:type(-1) == 'table')
end

-- Can we even test `error`??
-- TODO: Test: error
do
	assert(type(multilua.error) == 'function')
end

-- Can we even test `error`??
-- TODO: Test: error meta
do
	assert(type(multilua.error) == 'function')
end

-- Can we even test `gc`??
-- TODO: Test: gc
do
	assert(type(multilua.gc) == 'function')
end

-- Can we even test `gc`??
-- TODO: Test: gc meta
do
	assert(type(multilua.gc) == 'function')
end

-- TODO: Test: getfield
do
	assert(type(multilua.getfield) == 'function')
end

-- TODO: Test: getfield meta
do
	assert(type(multilua.getfield) == 'function')
end

-- Test: luaversion
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.luaversion) == 'function')
	assert(multilua.luaversion(obj) == 503.0)

	obj:close()
end

-- Test: luaversion meta
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.luaversion) == 'function')
	assert(obj:luaversion() == 503.0)

	obj:close()
end

-- TODO: Test: xmove
do
	assert(type(multilua.xmove) == 'function')
end

-- TODO: Test: xmove meta
do
	assert(type(multilua.xmove) == 'function')
end

-- TODO: Test: yield
do
	assert(type(multilua.yield) == 'function')
end

-- TODO: Test: yield meta
do
	assert(type(multilua.yield) == 'function')
end

-- TODO: Test: getglobal
do
	assert(type(multilua.getglobal) == 'function')
end

-- TODO: Test: getglobal meta
do
	assert(type(multilua.getglobal) == 'function')
end

-- TODO: Test: geti
do
	assert(type(multilua.geti) == 'function')
end

-- TODO: Test: geti meta
do
	assert(type(multilua.geti) == 'function')
end

-- TODO: Test: getmetatable
do
	assert(type(multilua.getmetatable) == 'function')
end

-- TODO: Test: getmetatable meta
do
	assert(type(multilua.getmetatable) == 'function')
end

-- TODO: Test: gettable
do
	assert(type(multilua.gettable) == 'function')
end

-- TODO: Test: gettable meta
do
	assert(type(multilua.gettable) == 'function')
end

-- TODO: Test: getuservalue
do
	assert(type(multilua.getuservalue) == 'function')
end

-- TODO: Test: getuservalue meta
do
	assert(type(multilua.getuservalue) == 'function')
end

-- TODO: Test: insert
do
	assert(type(multilua.insert) == 'function')
end

-- TODO: Test: insert meta
do
	assert(type(multilua.insert) == 'function')
end

-- TODO: Test: isboolean
do
	assert(type(multilua.isboolean) == 'function')
end

-- TODO: Test: isboolean meta
do
	assert(type(multilua.isboolean) == 'function')
end

-- Test: iscfunction
do
	assert(type(multilua.iscfunction) == 'function')

	local obj = multilua.new()

	multilua.pushcfunction(obj, multilua.new)
	assert(multilua.iscfunction(obj, -1))

	multilua.pushinteger(obj, 10)
	assert(multilua.iscfunction(obj, -1) == false)
end

-- Test: iscfunction meta
do
	assert(type(multilua.iscfunction) == 'function')

	local obj = multilua.new()
	assert(type(obj.iscfunction) == 'function')

	obj:pushcfunction(multilua.new)
	assert(obj:iscfunction(-1))

	obj:pushinteger(10)
	assert(obj:iscfunction(-1) == false)
end

-- Test: isfunction
do
	assert(type(multilua.isfunction) == 'function')

	local obj = multilua.new()

	multilua.dostring(obj, "return function() end")
	assert(multilua.isfunction(obj, -1))
end

-- Test: isfunction meta
do
	assert(type(multilua.isfunction) == 'function')

	local obj = multilua.new()
	assert(type(obj.isfunction) == "function")

	obj:dostring("return function() end")
	assert(obj:isfunction(-1))
end

-- TODO: Test: isinteger
do
	assert(type(multilua.isinteger) == 'function')
end

-- TODO: Test: isinteger meta
do
	assert(type(multilua.isinteger) == 'function')
end

-- TODO: Test: islightuserdata
do
	assert(type(multilua.islightuserdata) == 'function')
end

-- TODO: Test: islightuserdata meta
do
	assert(type(multilua.islightuserdata) == 'function')
end

-- TODO: Test: isnil
do
	assert(type(multilua.isnil) == 'function')
end

-- TODO: Test: isnil meta
do
	assert(type(multilua.isnil) == 'function')
end

-- TODO: Test: isnone
do
	assert(type(multilua.isnone) == 'function')
end

-- TODO: Test: isnone meta
do
	assert(type(multilua.isnone) == 'function')
end

-- TODO: Test: isnoneornil
do
	assert(type(multilua.isnoneornil) == 'function')
end

-- TODO: Test: isnoneornil meta
do
	assert(type(multilua.isnoneornil) == 'function')
end

-- TODO: Test: isnumber
do
	assert(type(multilua.isnumber) == 'function')
end

-- TODO: Test: isnumber meta
do
	assert(type(multilua.isnumber) == 'function')
end

-- TODO: Test: isstring
do
	assert(type(multilua.isstring) == 'function')
end

-- TODO: Test: isstring meta
do
	assert(type(multilua.isstring) == 'function')
end

-- TODO: Test: istable
do
	assert(type(multilua.istable) == 'function')
end

-- TODO: Test: istable meta
do
	assert(type(multilua.istable) == 'function')
end

-- TODO: Test: isthread
do
	assert(type(multilua.isthread) == 'function')
end

-- TODO: Test: isthread meta
do
	assert(type(multilua.isthread) == 'function')
end

-- TODO: Test: isuserdata
do
	assert(type(multilua.isuserdata) == 'function')
end

-- TODO: Test: isuserdata meta
do
	assert(type(multilua.isuserdata) == 'function')
end

-- TODO: Test: isyieldable
do
	assert(type(multilua.isyieldable) == 'function')
end

-- TODO: Test: isyieldable meta
do
	assert(type(multilua.isyieldable) == 'function')
end

-- TODO: Test: len
do
	assert(type(multilua.len) == 'function')
end

-- TODO: Test: len meta
do
	assert(type(multilua.len) == 'function')
end

-- TODO: Test: newtable
do
	assert(type(multilua.newtable) == 'function')
end

-- TODO: Test: newtable meta
do
	assert(type(multilua.newtable) == 'function')
end

-- TODO: Test: newthread
do
	assert(type(multilua.newthread) == 'function')
end

-- TODO: Test: newthread meta
do
	assert(type(multilua.newthread) == 'function')
end

-- TODO: Test: newuserdata
do
	assert(type(multilua.newuserdata) == 'function')
end

-- TODO: Test: newuserdata meta
do
	assert(type(multilua.newuserdata) == 'function')
end

-- TODO: Test: next
do
	assert(type(multilua.next) == 'function')
end

-- TODO: Test: next meta
do
	assert(type(multilua.next) == 'function')
end

-- TODO: Test: pcall
do
	assert(type(multilua.pcall) == 'function')
end

-- TODO: Test: pcall meta
do
	assert(type(multilua.pcall) == 'function')
end

-- TODO: Test: pop
do
	assert(type(multilua.pop) == 'function')
end

-- TODO: Test: pop meta
do
	assert(type(multilua.pop) == 'function')
end

-- TODO: Test: pushboolean
do
	assert(type(multilua.pushboolean) == 'function')
end

-- TODO: Test: pushboolean meta
do
	assert(type(multilua.pushboolean) == 'function')
end

-- TODO: Test: pushglobaltable
do
	assert(type(multilua.pushglobaltable) == 'function')
end

-- TODO: Test: pushglobaltable meta
do
	assert(type(multilua.pushglobaltable) == 'function')
end

-- TODO: Test: pushinteger
do
	assert(type(multilua.pushinteger) == 'function')
end

-- TODO: Test: pushinteger meta
do
	assert(type(multilua.pushinteger) == 'function')
end

-- TODO: Test: pushstring
do
	assert(type(multilua.pushstring) == 'function')
end

-- TODO: Test: pushstring meta
do
	assert(type(multilua.pushstring) == 'function')
end

-- TODO: Test: pushnstring
do
	assert(type(multilua.pushnstring) == 'function')
end

-- TODO: Test: pushnstring meta
do
	assert(type(multilua.pushnstring) == 'function')
end

-- TODO: Test: pushnil
do
	assert(type(multilua.pushnil) == 'function')
end

-- TODO: Test: pushnil meta
do
	assert(type(multilua.pushnil) == 'function')
end

-- TODO: Test: pushnumber
do
	assert(type(multilua.pushnumber) == 'function')
end

-- TODO: Test: pushnumber meta
do
	assert(type(multilua.pushnumber) == 'function')
end

-- TODO: Test: pushvalue
do
	assert(type(multilua.pushvalue) == 'function')
end

-- TODO: Test: pushvalue meta
do
	assert(type(multilua.pushvalue) == 'function')
end

-- TODO: Test: pushlightuserdata
do
	assert(type(multilua.pushlightuserdata) == 'function')
end

-- TODO: Test: pushlightuserdata meta
do
	assert(type(multilua.pushlightuserdata) == 'function')
end

-- TODO: Test: rawequal
do
	assert(type(multilua.rawequal) == 'function')
end

-- TODO: Test: rawequal meta
do
	assert(type(multilua.rawequal) == 'function')
end

-- TODO: Test: rawget
do
	assert(type(multilua.rawget) == 'function')
end

-- TODO: Test: rawget meta
do
	assert(type(multilua.rawget) == 'function')
end

-- TODO: Test: rawgeti
do
	assert(type(multilua.rawgeti) == 'function')
end

-- TODO: Test: rawgeti meta
do
	assert(type(multilua.rawgeti) == 'function')
end

-- TODO: Test: rawlen
do
	assert(type(multilua.rawlen) == 'function')
end

-- TODO: Test: rawlen meta
do
	assert(type(multilua.rawlen) == 'function')
end

-- TODO: Test: rawset
do
	assert(type(multilua.rawset) == 'function')
end

-- TODO: Test: rawset meta
do
	assert(type(multilua.rawset) == 'function')
end

-- TODO: Test: rawseti
do
	assert(type(multilua.rawseti) == 'function')
end

-- TODO: Test: rawseti meta
do
	assert(type(multilua.rawseti) == 'function')
end

-- TODO: Test: rawsetp
do
	assert(type(multilua.rawsetp) == 'function')
end

-- TODO: Test: rawsetp meta
do
	assert(type(multilua.rawsetp) == 'function')
end

-- TODO: Test: remove
do
	assert(type(multilua.remove) == 'function')
end

-- TODO: Test: remove meta
do
	assert(type(multilua.remove) == 'function')
end

-- TODO: Test: replace
do
	assert(type(multilua.replace) == 'function')
end

-- TODO: Test: replace meta
do
	assert(type(multilua.replace) == 'function')
end

-- TODO: Test: rotate
do
	assert(type(multilua.rotate) == 'function')
end

-- TODO: Test: rotate meta
do
	assert(type(multilua.rotate) == 'function')
end

-- TODO: Test: setfield
do
	assert(type(multilua.setfield) == 'function')
end

-- TODO: Test: setfield meta
do
	assert(type(multilua.setfield) == 'function')
end

-- TODO: Test: setglobal
do
	assert(type(multilua.setglobal) == 'function')
end

-- TODO: Test: setglobal meta
do
	assert(type(multilua.setglobal) == 'function')
end

-- TODO: Test: seti
do
	assert(type(multilua.seti) == 'function')
end

-- TODO: Test: seti meta
do
	assert(type(multilua.seti) == 'function')
end

-- TODO: Test: setmetatable
do
	assert(type(multilua.setmetatable) == 'function')
end

-- TODO: Test: setmetatable meta
do
	assert(type(multilua.setmetatable) == 'function')
end

-- TODO: Test: settable
do
	assert(type(multilua.settable) == 'function')
end

-- TODO: Test: settable meta
do
	assert(type(multilua.settable) == 'function')
end

-- TODO: Test: settop
do
	assert(type(multilua.settop) == 'function')
end

-- TODO: Test: settop meta
do
	assert(type(multilua.settop) == 'function')
end

-- TODO: Test: setuservalue
do
	assert(type(multilua.setuservalue) == 'function')
end

-- TODO: Test: setuservalue meta
do
	assert(type(multilua.setuservalue) == 'function')
end

-- TODO: Test: status
do
	assert(type(multilua.status) == 'function')
end

-- TODO: Test: status meta
do
	assert(type(multilua.status) == 'function')
end

-- TODO: Test: toboolean
do
	assert(type(multilua.toboolean) == 'function')
end

-- TODO: Test: toboolean meta
do
	assert(type(multilua.toboolean) == 'function')
end

-- TODO: Test: tointeger
do
	assert(type(multilua.tointeger) == 'function')
end

-- TODO: Test: tointeger meta
do
	assert(type(multilua.tointeger) == 'function')
end

-- TODO: Test: tointegerx
do
	assert(type(multilua.tointegerx) == 'function')
end

-- TODO: Test: tointegerx meta
do
	assert(type(multilua.tointegerx) == 'function')
end

-- TODO: Test: tostring
do
	assert(type(multilua.tostring) == 'function')
end

-- TODO: Test: tostring meta
do
	assert(type(multilua.tostring) == 'function')
end

-- TODO: Test: tonumber
do
	assert(type(multilua.tonumber) == 'function')
end

-- TODO: Test: tonumber meta
do
	assert(type(multilua.tonumber) == 'function')
end

-- TODO: Test: tonumberx
do
	assert(type(multilua.tonumberx) == 'function')
end

-- TODO: Test: tonumberx meta
do
	assert(type(multilua.tonumberx) == 'function')
end

-- TODO: Test: topointer
do
	assert(type(multilua.topointer) == 'function')
end

-- TODO: Test: topointer meta
do
	assert(type(multilua.topointer) == 'function')
end

-- TODO: Test: tothread
do
	assert(type(multilua.tothread) == 'function')
end

-- TODO: Test: tothread meta
do
	assert(type(multilua.tothread) == 'function')
end

-- TODO: Test: touserdata
do
	assert(type(multilua.touserdata) == 'function')
end

-- TODO: Test: touserdata meta
do
	assert(type(multilua.touserdata) == 'function')
end

-- TODO: Test: type
do
	assert(type(multilua.type) == 'function')
end

-- TODO: Test: type meta
do
	assert(type(multilua.type) == 'function')
end

-- TODO: Test: gethookcount
do
	assert(type(multilua.gethookcount) == 'function')
end

-- TODO: Test: gethookcount meta
do
	assert(type(multilua.gethookcount) == 'function')
end

-- TODO: Test: gethookmask
do
	assert(type(multilua.gethookmask) == 'function')
end

-- TODO: Test: gethookmask meta
do
	assert(type(multilua.gethookmask) == 'function')
end

-- TODO: Test: getupvalue
do
	assert(type(multilua.getupvalue) == 'function')
end

-- TODO: Test: getupvalue meta
do
	assert(type(multilua.getupvalue) == 'function')
end

-- TODO: Test: setupvalue
do
	assert(type(multilua.setupvalue) == 'function')
end

-- TODO: Test: setupvalue meta
do
	assert(type(multilua.setupvalue) == 'function')
end

-- TODO: Test: upvalueid
do
	assert(type(multilua.upvalueid) == 'function')
end

-- TODO: Test: upvalueid meta
do
	assert(type(multilua.upvalueid) == 'function')
end

-- TODO: Test: upvaluejoin
do
	assert(type(multilua.upvaluejoin) == 'function')
end

-- TODO: Test: upvaluejoin meta
do
	assert(type(multilua.upvaluejoin) == 'function')
end

-- TODO: Test: argcheck
do
	assert(type(multilua.argcheck) == 'function')
end

-- TODO: Test: argcheck meta
do
	assert(type(multilua.argcheck) == 'function')
end

-- TODO: Test: argerror
do
	assert(type(multilua.argerror) == 'function')
end

-- TODO: Test: argerror meta
do
	assert(type(multilua.argerror) == 'function')
end

-- TODO: Test: callmeta
do
	assert(type(multilua.callmeta) == 'function')
end

-- TODO: Test: callmeta meta
do
	assert(type(multilua.callmeta) == 'function')
end

-- TODO: Test: checkany
do
	assert(type(multilua.checkany) == 'function')
end

-- TODO: Test: checkany meta
do
	assert(type(multilua.checkany) == 'function')
end

-- TODO: Test: checkinteger
do
	assert(type(multilua.checkinteger) == 'function')
end

-- TODO: Test: checkinteger meta
do
	assert(type(multilua.checkinteger) == 'function')
end

-- TODO: Test: checkstring
do
	assert(type(multilua.checkstring) == 'function')
end

-- TODO: Test: checkstring meta
do
	assert(type(multilua.checkstring) == 'function')
end

-- TODO: Test: checknumber
do
	assert(type(multilua.checknumber) == 'function')
end

-- TODO: Test: checknumber meta
do
	assert(type(multilua.checknumber) == 'function')
end

-- TODO: Test: lcheckstack
do
	assert(type(multilua.lcheckstack) == 'function')
end

-- TODO: Test: lcheckstack meta
do
	assert(type(multilua.lcheckstack) == 'function')
end

-- TODO: Test: checktype
do
	assert(type(multilua.checktype) == 'function')
end

-- TODO: Test: checktype meta
do
	assert(type(multilua.checktype) == 'function')
end

-- TODO: Test: checkudata
do
	assert(type(multilua.checkudata) == 'function')
end

-- TODO: Test: checkudata meta
do
	assert(type(multilua.checkudata) == 'function')
end

-- TODO: Test: checkversion
do
	assert(type(multilua.checkversion) == 'function')
end

-- TODO: Test: checkversion meta
do
	assert(type(multilua.checkversion) == 'function')
end

-- TODO: Test: dofile
do
	assert(type(multilua.dofile) == 'function')
end

-- TODO: Test: dofile meta
do
	assert(type(multilua.dofile) == 'function')
end

-- TODO: Test: dostring
do
	assert(type(multilua.dostring) == 'function')
end

-- TODO: Test: dostring meta
do
	assert(type(multilua.dostring) == 'function')
end

-- TODO: Test: execresult
do
	assert(type(multilua.execresult) == 'function')
end

-- TODO: Test: execresult meta
do
	assert(type(multilua.execresult) == 'function')
end

-- TODO: Test: fileresult
do
	assert(type(multilua.fileresult) == 'function')
end

-- TODO: Test: fileresult meta
do
	assert(type(multilua.fileresult) == 'function')
end

-- TODO: Test: getmetafield
do
	assert(type(multilua.getmetafield) == 'function')
end

-- TODO: Test: getmetafield meta
do
	assert(type(multilua.getmetafield) == 'function')
end

-- TODO: Test: lgetmetatable
do
	assert(type(multilua.lgetmetatable) == 'function')
end

-- TODO: Test: lgetmetatable meta
do
	assert(type(multilua.lgetmetatable) == 'function')
end

-- TODO: Test: getsubtable
do
	assert(type(multilua.getsubtable) == 'function')
end

-- TODO: Test: getsubtable meta
do
	assert(type(multilua.getsubtable) == 'function')
end

-- TODO: Test: gsub
do
	assert(type(multilua.gsub) == 'function')
end

-- TODO: Test: gsub meta
do
	assert(type(multilua.gsub) == 'function')
end

-- TODO: Test: llen
do
	assert(type(multilua.llen) == 'function')
end

-- TODO: Test: llen meta
do
	assert(type(multilua.llen) == 'function')
end

-- TODO: Test: loadbuffer
do
	assert(type(multilua.loadbuffer) == 'function')
end

-- TODO: Test: loadbuffer meta
do
	assert(type(multilua.loadbuffer) == 'function')
end

-- TODO: Test: loadbufferx
do
	assert(type(multilua.loadbufferx) == 'function')
end

-- TODO: Test: loadbufferx meta
do
	assert(type(multilua.loadbufferx) == 'function')
end

-- TODO: Test: loadfile
do
	assert(type(multilua.loadfile) == 'function')
end

-- TODO: Test: loadfile meta
do
	assert(type(multilua.loadfile) == 'function')
end

-- TODO: Test: loadfilex
do
	assert(type(multilua.loadfilex) == 'function')
end

-- TODO: Test: loadfilex meta
do
	assert(type(multilua.loadfilex) == 'function')
end

-- TODO: Test: loadstring
do
	assert(type(multilua.loadstring) == 'function')
end

-- TODO: Test: loadstring meta
do
	assert(type(multilua.loadstring) == 'function')
end

-- TODO: Test: newmetatable
do
	assert(type(multilua.newmetatable) == 'function')
end

-- TODO: Test: newmetatable meta
do
	assert(type(multilua.newmetatable) == 'function')
end

-- TODO: Test: optinteger
do
	assert(type(multilua.optinteger) == 'function')
end

-- TODO: Test: optinteger meta
do
	assert(type(multilua.optinteger) == 'function')
end

-- TODO: Test: optstring
do
	assert(type(multilua.optstring) == 'function')
end

-- TODO: Test: optstring meta
do
	assert(type(multilua.optstring) == 'function')
end

-- TODO: Test: optnumber
do
	assert(type(multilua.optnumber) == 'function')
end

-- TODO: Test: optnumber meta
do
	assert(type(multilua.optnumber) == 'function')
end

-- TODO: Test: ref
do
	assert(type(multilua.ref) == 'function')
end

-- TODO: Test: ref meta
do
	assert(type(multilua.ref) == 'function')
end

-- Test: refnil
do
	assert(type(multilua.refnil) == 'function')
	assert(type(multilua.refnil()) == 'number')
end

-- Test: refnil meta
do
	local obj = multilua.new()
	assert(type(obj.refnil) == 'function')
	assert(type(obj:refnil()) == 'number')

	obj:close()
end

-- Test: noref
do
	assert(type(multilua.noref) == 'function')
	assert(type(multilua.noref()) == 'number')
end

-- Test: noref meta
do
	local obj = multilua.new()
	assert(type(obj.noref) == 'function')
	assert(type(obj:noref()) == 'number')

	obj:close()
end

-- TODO: Test: lsetmetatable
do
	assert(type(multilua.lsetmetatable) == 'function')
end

-- TODO: Test: lsetmetatable meta
do
	assert(type(multilua.lsetmetatable) == 'function')
end

-- TODO: Test: testudata
do
	assert(type(multilua.testudata) == 'function')
end

-- TODO: Test: testudata meta
do
	assert(type(multilua.testudata) == 'function')
end

-- TODO: Test: ltostring
do
	assert(type(multilua.ltostring) == 'function')
end

-- TODO: Test: ltostring meta
do
	assert(type(multilua.ltostring) == 'function')
end

-- TODO: Test: traceback
do
	assert(type(multilua.traceback) == 'function')
end

-- TODO: Test: traceback meta
do
	assert(type(multilua.traceback) == 'function')
end

-- TODO: Test: unref
do
	assert(type(multilua.unref) == 'function')
end

-- TODO: Test: unref meta
do
	assert(type(multilua.unref) == 'function')
end

-- TODO: Test: where
do
	assert(type(multilua.where) == 'function')
end

-- TODO: Test: where meta
do
	assert(type(multilua.where) == 'function')
end

-- TODO: Test: resume
do
	assert(type(multilua.resume) == 'function')
end

-- TODO: Test: resume meta
do
	assert(type(multilua.resume) == 'function')
end

-- TODO: Test: rawgetp
do
	assert(type(multilua.rawgetp) == 'function')
end

-- TODO: Test: rawgetp meta
do
	assert(type(multilua.rawgetp) == 'function')
end

-- TODO: Test: pushthread
do
	assert(type(multilua.pushthread) == 'function')
end

-- TODO: Test: pushthread meta
do
	assert(type(multilua.pushthread) == 'function')
end

-- TODO: Test: tocfunction
do
	assert(type(multilua.tocfunction) == 'function')
end

-- TODO: Test: tocfunction meta
do
	assert(type(multilua.tocfunction) == 'function')
end

-- TODO: Test: pushcclosure
do
	assert(type(multilua.pushcclosure) == 'function')
end

-- TODO: Test: pushcclosure meta
do
	assert(type(multilua.pushcclosure) == 'function')
end

-- Test: pushcfunction
do
	assert(type(multilua.pushcfunction) == 'function')

	local obj = multilua.new()

	assert(multilua.pushcfunction(obj, multilua.new))
	assert(multilua.iscfunction(obj, -1))
end

-- Test: pushcfunction meta
do
	assert(type(multilua.pushcfunction) == 'function')

	local obj = multilua.new()

	assert(obj:pushcfunction(multilua.new))
	assert(obj:iscfunction(-1))
end

-- TODO: Test: newreg
do
	assert(type(multilua.newreg) == 'function')
end

-- TODO: Test: newreg meta
do
	assert(type(multilua.newreg) == 'function')
end

-- TODO: Test: tabletoreg
do
	assert(type(multilua.tabletoreg) == 'function')
end

-- TODO: Test: tabletoreg meta
do
	assert(type(multilua.tabletoreg) == 'function')
end

-- TODO: Test: setfuncs
do
	assert(type(multilua.setfuncs) == 'function')
end

-- TODO: Test: setfuncs meta
do
	assert(type(multilua.setfuncs) == 'function')
end

-- TODO: Test: requiref
do
	assert(type(multilua.requiref) == 'function')
end

-- TODO: Test: requiref meta
do
	assert(type(multilua.requiref) == 'function')
end

-- TODO: Test: buffinit
do
	assert(type(multilua.buffinit) == 'function')
end

-- TODO: Test: buffinit meta
do
	assert(type(multilua.buffinit) == 'function')
end

-- TODO: Test: buffinitsize
do
	assert(type(multilua.buffinitsize) == 'function')
end

-- TODO: Test: buffinitsize meta
do
	assert(type(multilua.buffinitsize) == 'function')
end

-- TODO: Test: prepbuffsize
do
	assert(type(multilua.prepbuffsize) == 'function')
end

-- TODO: Test: prepbuffsize meta
do
	assert(type(multilua.prepbuffsize) == 'function')
end

-- TODO: Test: prepbuffer
do
	assert(type(multilua.prepbuffer) == 'function')
end

-- TODO: Test: prepbuffer meta
do
	assert(type(multilua.prepbuffer) == 'function')
end

-- TODO: Test: addchar
do
	assert(type(multilua.addchar) == 'function')
end

-- TODO: Test: addchar meta
do
	assert(type(multilua.addchar) == 'function')
end

-- TODO: Test: addstring
do
	assert(type(multilua.addstring) == 'function')
end

-- TODO: Test: addstring meta
do
	assert(type(multilua.addstring) == 'function')
end

-- TODO: Test: addsize
do
	assert(type(multilua.addsize) == 'function')
end

-- TODO: Test: addsize meta
do
	assert(type(multilua.addsize) == 'function')
end

-- TODO: Test: addvalue
do
	assert(type(multilua.addvalue) == 'function')
end

-- TODO: Test: addvalue meta
do
	assert(type(multilua.addvalue) == 'function')
end

-- TODO: Test: pushresult
do
	assert(type(multilua.pushresult) == 'function')
end

-- TODO: Test: pushresult meta
do
	assert(type(multilua.pushresult) == 'function')
end

-- TODO: Test: pushresultsize
do
	assert(type(multilua.pushresultsize) == 'function')
end

-- TODO: Test: pushresultsize meta
do
	assert(type(multilua.pushresultsize) == 'function')
end

-- TODO: Test: newlib
do
	assert(type(multilua.newlib) == 'function')
end

-- TODO: Test: newlib meta
do
	assert(type(multilua.newlib) == 'function')
end

-- TODO: Test: register
do
	assert(type(multilua.register) == 'function')
end

-- TODO: Test: register meta
do
	assert(type(multilua.register) == 'function')
end

-- TODO: Test: atpanic
do
	assert(type(multilua.atpanic) == 'function')
end

-- TODO: Test: atpanic meta
do
	assert(type(multilua.atpanic) == 'function')
end

-- TODO: Test: load
do
	assert(type(multilua.load) == 'function')
end

-- TODO: Test: load meta
do
	assert(type(multilua.load) == 'function')
end

-- TODO: Test: getallocf
do
	assert(type(multilua.getallocf) == 'function')
end

-- TODO: Test: getallocf meta
do
	assert(type(multilua.getallocf) == 'function')
end

-- TODO: Test: setallocf
do
	assert(type(multilua.setallocf) == 'function')
end

-- TODO: Test: setallocf meta
do
	assert(type(multilua.setallocf) == 'function')
end

-- TODO: Test: getextraspace
do
	assert(type(multilua.getextraspace) == 'function')
end

-- TODO: Test: getextraspace meta
do
	assert(type(multilua.getextraspace) == 'function')
end

-- TODO: Test: pcallk
do
	assert(type(multilua.pcallk) == 'function')
end

-- TODO: Test: pcallk meta
do
	assert(type(multilua.pcallk) == 'function')
end

-- TODO: Test: yieldk
do
	assert(type(multilua.yieldk) == 'function')
end

-- TODO: Test: yieldk meta
do
	assert(type(multilua.yieldk) == 'function')
end

-- TODO: Test: gethook
do
	assert(type(multilua.gethook) == 'function')
end

-- TODO: Test: gethook meta
do
	assert(type(multilua.gethook) == 'function')
end

-- TODO: Test: sethook
do
	assert(type(multilua.sethook) == 'function')
end

-- TODO: Test: sethook meta
do
	assert(type(multilua.sethook) == 'function')
end

-- TODO: Test: checkoption
do
	assert(type(multilua.checkoption) == 'function')
end

-- TODO: Test: checkoption meta
do
	assert(type(multilua.checkoption) == 'function')
end

-- Test self-library manipulation...
do
	-- Test: Creation
	local obj = multilua.current()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	-- Closing the current state is ignored:
	-- (This will segfault if it fails...)
	assert(multilua.close(obj) == nil)

	-- Closing the current state is ignored:
	-- (This will segfault if it fails...)
	obj = multilua.current()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')
	assert(obj:close() == nil)
end
