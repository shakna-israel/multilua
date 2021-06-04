local multilua = require "multilua"

-- Test: We got a library
assert(multilua ~= nil)
assert(type(multilua) == 'table')

-- Test: We have a library version
assert(multilua.version ~= nil)

-- Test: creation
do
	assert(type(multilua.new) == 'function')
	local obj = assert(multilua.new())
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	-- Test: self has a metatable
	local t = assert(getmetatable(obj.self))
	assert(t['__gc'])

	-- Test: closing
	assert(type(multilua.close) == 'function')
	assert(type(multilua.close(obj)) ~= 'nil')
	assert(obj.self == nil)
end

-- Test: GC
do
	local obj = assert(multilua.new())
	getmetatable(obj.self)['__gc'](obj.self)
end

-- Test: creation meta
do
	assert(type(multilua.new) == 'function')
	local obj = multilua()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	-- Test: self has a metatable
	assert(getmetatable(obj.self))

	-- Test: closing
	assert(type(multilua.close) == 'function')
	assert(type(multilua.close(obj)) ~= 'nil')
	assert(obj.self == nil)
end

-- Test: Multiple closes
-- If this fails, you'll get a double-free!
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.close(obj)) ~= 'nil')
	assert(obj.self == nil)
	assert(type(multilua.close(obj)) == 'nil')
	assert(obj.self == nil)
end

-- Test: metaclosing
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')
	assert(obj:close() ~= nil)
end

-- Test: Closing current state is a no-op
do
	local obj = multilua.current()
	assert(multilua.close(obj) == nil)
	assert(obj.self)
end

-- Test: Closing current state is a no-op meta
do
	local obj = multilua.current()
	assert(obj:close() == nil)
	assert(obj.self)
end

-- Test multiple states can exist
-- simultaneously
do
	local obj1 = multilua.new()
	local obj2 = multilua.new()

	assert(obj1.self ~= obj2.self)

	assert(obj1:close())
	assert(obj2:close())
end

-- Test equivalents
do
	local obj1 = multilua.new()
	local obj2 = multilua.new()

	assert(obj1 == obj1)
	assert(obj1 ~= obj2)

	assert(obj1:close())
	assert(obj2:close())
end

-- Test newindex metamethod
do
	local obj = assert(multilua.new())

	-- Pushing numbers...
	obj[#obj + 1] = 10
	assert(obj[#obj] == "number")

	-- Pushing booleans
	obj[#obj + 1] = true
	assert(obj[#obj] == "boolean")

	-- Pushing strings
	obj[#obj + 1] = "hello world"
	assert(obj[#obj] == "string")

	-- Push table reference
	obj[#obj + 1] = {}
	assert(obj[#obj] == 'table')

	-- Push Lua function reference
	obj[#obj + 1] = function() end
	assert(obj[#obj] == 'function')

	-- Pushing C Functions
	obj[#obj + 1] = multilua.new
	assert(obj[#obj] == "function")

	-- Push a full userdata reference
	local f = io.open("test.lua", "r")
	obj[#obj + 1] = f
	assert(obj[#obj] == 'userdata')

	-- Push a thread reference
	local co = coroutine.create(function() end)
	obj[#obj + 1] = co
	assert(obj[#obj] == 'thread')

	-- Pushing lightuserdata is allowed
	obj[#obj + 1] = obj['self']
	assert(obj[#obj] == 'userdata')

	-- Can still modify fields:
	obj['hello'] = 'world'
	assert(obj['hello'] == 'world')

	assert(obj:close())
end

-- Test tostring name
do
	local obj = assert(multilua.new())
	assert(tostring(obj):sub(1, 9) == "multilua:")

	assert(obj:close())
end

-- Test copying values by call
do
	local obj = assert(multilua.new())

	-- Calling on nothing to get nil
	assert(obj(-1) == nil)

	-- Calling to get nil
	obj[#obj + 1] = nil
	assert(obj(-1) == nil)

	-- Calling to get an integer
	obj[#obj + 1] = 10
	assert(obj(-1) == 10)

	-- Calling to get a float
	obj[#obj + 1] = 1.3
	assert(obj(-1) == 1.3)

	-- Calling to get a boolean
	obj[#obj + 1] = false
	assert(obj(-1) == false)
	obj[#obj + 1] = true
	assert(obj(-1) == true)

	-- Calling to get a string
	obj[#obj + 1] = "Hello, World!"
	assert(obj(-1) == "Hello, World!")

	-- Can fetch table references:
	obj:newtable()
	assert(type(obj(-1)) == 'table')

	-- Calling to get a C Function
	obj[#obj + 1] = multilua.new
	assert(type(obj(-1)) == 'function')

	-- Can get a Lua function
	obj:dostring("return function() end")
	assert(type(obj(-1)) == 'function')

	-- Can get a full userdata reference
	assert(obj:openlibs())
	obj:dostring("return io.open('test.lua', 'r')")
	assert(type(obj(-1)) == 'userdata')

	-- Can get a thread reference
	assert(obj:openlibs())
	obj:dostring("return coroutine.create(function() end)")
	assert(type(obj(-1)) == 'thread')

	-- Calling to get lightuserdata
	obj[#obj + 1] = obj.self
	assert(obj(-1) == obj.self)

	assert(obj:close())
end

-- Test fetchable
do
	assert(type(multilua.fetchable) == 'function')

	local obj = assert(multilua.new())

	-- Testing: none (must come first)
	assert(multilua.fetchable(obj, -1))

	-- nil
	multilua.pushnil(obj)
	assert(multilua.fetchable(obj, -1))

	-- integer
	multilua.pushinteger(obj, 10)
	assert(multilua.fetchable(obj, -1))

	-- float
	multilua:pushnumber(obj, 1.2)
	assert(multilua.fetchable(obj, -1))

	-- boolean
	multilua.pushboolean(obj, false)
	assert(multilua.fetchable(obj, -1))

	-- string
	multilua.pushstring(obj, "Hello")
	assert(multilua.fetchable(obj, -1))

	-- table (reference type)
	multilua.newtable(obj)
	assert(multilua.fetchable(obj, -1) == false)

	-- c function
	multilua.pushcfunction(obj, multilua.new)
	assert(multilua.fetchable(obj, -1))

	-- lua function (reference type)
	multilua.dostring(obj, "return function() end")
	assert(multilua.fetchable(obj, -1) == false)

	assert(multilua.openlibs(obj))

	-- full userdata (reference type)
	multilua.dostring(obj, "return io.open('test.lua', 'r')")
	assert(multilua.fetchable(obj, -1) == false)

	-- thread (reference type)
	multilua.dostring(obj, "return coroutine.create(function() end)")
	assert(multilua.fetchable(obj, -1) == false)

	-- lightuserdata
	multilua.pushlightuserdata(obj, obj.self)
	assert(multilua.fetchable(obj, -1))

	assert(obj:close())
end

-- Test fetchable meta
do
	assert(type(multilua.fetchable) == 'function')

	local obj = assert(multilua.new())

	-- Testing: none (must come first)
	assert(obj:fetchable(-1))

	-- nil
	multilua.pushnil(obj)
	assert(obj:fetchable(-1))

	-- integer
	multilua.pushinteger(obj, 10)
	assert(obj:fetchable(-1))

	-- float
	multilua:pushnumber(obj, 1.2)
	assert(obj:fetchable(-1))

	-- boolean
	multilua.pushboolean(obj, false)
	assert(obj:fetchable(-1))

	-- string
	multilua.pushstring(obj, "Hello")
	assert(obj:fetchable(-1))

	-- table (reference type)
	multilua.newtable(obj)
	assert(obj:fetchable(-1) == false)

	-- c function
	multilua.pushcfunction(obj, multilua.new)
	assert(obj:fetchable(-1))

	-- lua function (reference type)
	multilua.dostring(obj, "return function() end")
	assert(obj:fetchable(-1) == false)

	assert(multilua.openlibs(obj))

	-- full userdata (reference type)
	multilua.dostring(obj, "return io.open('test.lua', 'r')")
	assert(obj:fetchable(-1) == false)

	-- thread (reference type)
	multilua.dostring(obj, "return coroutine.create(function() end)")
	assert(obj:fetchable(-1) == false)

	-- lightuserdata
	multilua.pushlightuserdata(obj, obj.self)
	assert(obj:fetchable(-1))

	assert(obj:close())
end

-- Test fetchable without explicit index
do
	assert(type(multilua.fetchable) == 'function')

	local obj = assert(multilua.new())

	-- Testing: none (must come first)
	assert(multilua.fetchable(obj))

	-- nil
	multilua.pushnil(obj)
	assert(multilua.fetchable(obj))

	-- integer
	multilua.pushinteger(obj, 10)
	assert(multilua.fetchable(obj))

	-- float
	multilua:pushnumber(obj, 1.2)
	assert(multilua.fetchable(obj))

	-- boolean
	multilua.pushboolean(obj, false)
	assert(multilua.fetchable(obj))

	-- string
	multilua.pushstring(obj, "Hello")
	assert(multilua.fetchable(obj))

	-- table (reference type)
	multilua.newtable(obj)
	assert(multilua.fetchable(obj) == false)

	-- c function
	multilua.pushcfunction(obj, multilua.new)
	assert(multilua.fetchable(obj))

	-- lua function (reference type)
	multilua.dostring(obj, "return function() end")
	assert(multilua.fetchable(obj) == false)

	assert(multilua.openlibs(obj))

	-- full userdata (reference type)
	multilua.dostring(obj, "return io.open('test.lua', 'r')")
	assert(multilua.fetchable(obj) == false)

	-- thread (reference type)
	multilua.dostring(obj, "return coroutine.create(function() end)")
	assert(multilua.fetchable(obj) == false)

	-- lightuserdata
	multilua.pushlightuserdata(obj, obj.self)
	assert(multilua.fetchable(obj))

	assert(obj:close())
end

-- Test fetchable meta without explicit index
do
	assert(type(multilua.fetchable) == 'function')

	local obj = assert(multilua.new())

	-- Testing: none (must come first)
	assert(obj:fetchable())

	-- nil
	multilua.pushnil(obj)
	assert(obj:fetchable())

	-- integer
	multilua.pushinteger(obj, 10)
	assert(obj:fetchable())

	-- float
	multilua:pushnumber(obj, 1.2)
	assert(obj:fetchable())

	-- boolean
	multilua.pushboolean(obj, false)
	assert(obj:fetchable())

	-- string
	multilua.pushstring(obj, "Hello")
	assert(obj:fetchable())

	-- table (reference type)
	multilua.newtable(obj)
	assert(obj:fetchable() == false)

	-- c function
	multilua.pushcfunction(obj, multilua.new)
	assert(obj:fetchable())

	-- lua function (reference type)
	multilua.dostring(obj, "return function() end")
	assert(obj:fetchable() == false)

	assert(multilua.openlibs(obj))

	-- full userdata (reference type)
	multilua.dostring(obj, "return io.open('test.lua', 'r')")
	assert(obj:fetchable() == false)

	-- thread (reference type)
	multilua.dostring(obj, "return coroutine.create(function() end)")
	assert(obj:fetchable() == false)

	-- lightuserdata
	multilua.pushlightuserdata(obj, obj.self)
	assert(obj:fetchable())

	assert(obj:close())
end

-- Test openlibs
do
	assert(type(multilua.openlibs) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openlibs(obj))

	-- String Library now exists:
	multilua.getglobal(obj, "string")
	t = multilua.type(obj, -1)
	assert(t == 'table')

	assert(obj:close())
end

-- Test: openlibs meta
do
	assert(type(multilua.openlibs) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.openlibs) == 'function')
	assert(obj:openlibs())

	-- String Library now exists:
	obj:getglobal("string")
	t = obj:type(-1)
	assert(t == 'table')

	assert(obj:close())
end

-- Test: gettop
do
	assert(type(multilua.gettop) == 'function')

	local obj = assert(multilua.new())

	for i = 1, 10 do
		multilua.pushinteger(obj, i)
	end

	assert(multilua.gettop(obj) == 10)

	assert(obj:close())
end

-- Test: gettop meta
do
	assert(type(multilua.gettop) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.gettop) == 'function')

	for i = 1, 10 do
		obj:pushinteger(i)
	end

	assert(obj:gettop() == 10)

	assert(obj:close())
end

-- Test: #
do
	assert(type(multilua.gettop) == 'function')

	local obj = assert(multilua.new())

	for i = 1, 10 do
		multilua.pushinteger(obj, i)
	end

	assert(#obj == 10)

	assert(obj:close())
end

-- Test: []
do
	assert(type(multilua.gettop) == 'function')

	local obj = assert(multilua.new())

	obj:pushinteger(10)
	obj:pushstring("hello")
	obj:pushboolean(false)

	assert(obj[1] == "number")
	assert(obj[2] == "string")
	assert(obj[3] == "boolean")

	assert(obj:close())
end

-- Test: absindex
do
	assert(type(multilua.absindex) == 'function')

	local obj = assert(multilua.new())

	-- Nothing on stack, should be 0.
	assert(multilua.absindex(obj, -1) == 0)

	multilua.pushinteger(obj, 1)

	-- Number on stack, should be 0.
	assert(multilua.absindex(obj, -1) == 1)

	assert(obj:close())
end

-- Test: absindex meta
do
	assert(type(multilua.absindex) == 'function')

	local obj = assert(multilua.new())

	-- Nothing on stack, should be 0.
	assert(obj:absindex(-1) == 0)

	obj:pushinteger(1)

	-- Number on stack, should be 0.
	assert(obj:absindex(-1) == 1)

	assert(obj:close())
end

-- Test: arith
do
	assert(type(multilua.arith) == 'function')

	local obj = assert(multilua.new())

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

	assert(obj:close())
end

-- Test: arith meta
do
	assert(type(multilua.arith) == 'function')

	local obj = assert(multilua.new())
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

	assert(obj:close())
end

-- Test: call
do
	assert(type(multilua.call) == 'function')

	local obj = assert(multilua.new())

	multilua.dostring(obj, "return function(a, b) return a + b end");
	multilua.pushinteger(obj, 10)
	multilua.pushinteger(obj, 10)
	assert(multilua.call(obj, 2, 1))
	assert(obj(-1) == 20)

	assert(obj:close())
end

-- Test: call meta
do
	assert(type(multilua.call) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.call) == 'function')

	obj:dostring("return function(a, b) return a + b end");
	obj:pushinteger(10)
	obj:pushinteger(10)
	assert(obj:call(2, 1))
	assert(obj(-1) == 20)

	assert(obj:close())
end

-- Test: call with optional return count
do
	assert(type(multilua.call) == 'function')

	local obj = assert(multilua.new())

	multilua.dostring(obj, "return function(a, b) return a + b end");
	multilua.pushinteger(obj, 10)
	multilua.pushinteger(obj, 10)
	assert(multilua.call(obj, 2))
	assert(obj(-1) == 20)

	assert(obj:close())
end

-- Test: call meta with optional return count
do
	assert(type(multilua.call) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.call) == 'function')

	obj:dostring("return function(a, b) return a + b end");
	obj:pushinteger(10)
	obj:pushinteger(10)
	assert(obj:call(2))
	assert(obj(-1) == 20)

	assert(obj:close())
end

-- Test: checkstack
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.checkstack) == 'function')
	assert(multilua.checkstack(obj, 2) == true)

	assert(obj:close())
end

-- Test: checkstack meta
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.checkstack) == 'function')
	assert(obj:checkstack(2) == true)

	assert(obj:close())
end

-- Test: compare
do
	assert(type(multilua.compare) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	multilua.pushinteger(obj, 11)

	assert(multilua.compare(obj, -1, -2, "==") == false)
	assert(multilua.compare(obj, -1, -2, "<") == false)
	assert(multilua.compare(obj, -1, -2, "<=") == false)

	assert(multilua.compare(obj, -2, -1, "==") == false)
	assert(multilua.compare(obj, -2, -1, "<") == true)
	assert(multilua.compare(obj, -2, -1, "<=") == true)

	assert(obj:close())
end

-- Test: compare meta
do
	assert(type(multilua.compare) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.compare) == 'function')

	obj:pushinteger(10)
	obj:pushinteger(11)
	
	assert(obj:compare(-1, -2, "==") == false)
	assert(obj:compare(-1, -2, "<") == false)
	assert(obj:compare(-1, -2, "<=") == false)

	assert(obj:compare(-2, -1, "==") == false)
	assert(obj:compare(-2, -1, "<") == true)
	assert(obj:compare(-2, -1, "<=") == true)

	assert(obj:close())
end

-- Test: concat
do
	assert(type(multilua.concat) == 'function')

	local obj = assert(multilua.new())

	multilua.pushstring(obj, "h")
	multilua.pushstring(obj, "e")
	multilua.pushstring(obj, "l")
	multilua.pushstring(obj, "l")
	multilua.pushstring(obj, "o")

	assert(multilua.concat(obj, 5))
	assert(multilua.tostring(obj, -1) == "hello")

	assert(obj:close())
end

-- Test: concat meta
do
	assert(type(multilua.concat) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.concat) == 'function')

	obj:pushstring("h")
	obj:pushstring("e")
	obj:pushstring("l")
	obj:pushstring("l")
	obj:pushstring("o")

	assert(obj:concat(5))
	assert(obj:tostring(-1) == "hello")

	assert(obj:close())
end

-- Test: copy
do
	assert(type(multilua.copy) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 3)

	assert(multilua.copy(obj, -1, -2))
	assert(multilua.tointeger(obj, -2) == 3)

	assert(obj:close())
end

-- Test: copy meta
do
	assert(type(multilua.copy) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.copy) == 'function')

	obj:pushinteger(3)

	assert(obj:copy(-1, -2))
	assert(obj:tointeger(-2) == 3)

	assert(obj:close())
end

-- Test: createtable
do
	assert(type(multilua.createtable) == 'function')

	local obj = assert(multilua.new())

	multilua.createtable(obj)
	assert(multilua.type(obj, -1) == 'table')

	multilua.createtable(obj, 10)
	assert(multilua.type(obj, -1) == 'table')

	multilua.createtable(obj, nil, 10)
	assert(multilua.type(obj, -1) == 'table')

	multilua.createtable(obj, 10, 10)
	assert(multilua.type(obj, -1) == 'table')

	assert(obj:close())
end

-- Test: createtable meta
do
	assert(type(multilua.createtable) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.createtable) == 'function')

	obj:createtable()
	assert(obj:type(-1) == 'table')

	obj:createtable(10)
	assert(obj:type(-1) == 'table')

	obj:createtable(nil, 10)
	assert(obj:type(-1) == 'table')

	obj:createtable(10, 10)
	assert(obj:type(-1) == 'table')

	assert(obj:close())
end

-- Test: error
do
	assert(type(multilua.error) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: error meta
do
	assert(type(multilua.error) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.error) == "function")

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--

	assert(obj:close())
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

-- Test: getfield
do
	assert(type(multilua.getfield) == 'function')

	local obj = assert(multilua.new())

	multilua.newtable(obj)
	multilua.pushstring(obj, "key")
	multilua.pushstring(obj, "value")
	multilua.rawset(obj, 1)

	assert(multilua.getfield(obj, -1, "key") == 'string')
	assert(obj(-1) == 'value')

	assert(obj:close())
end

-- Test: getfield meta
do
	assert(type(multilua.getfield) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.getfield) == 'function')

	obj:newtable()
	obj:pushstring("key")
	obj:pushstring("value")
	obj:rawset(1)

	assert(obj:getfield(-1, "key") == 'string')
	assert(obj(-1) == 'value')

	assert(obj:close())
end

-- Test: luaversion
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.luaversion) == 'function')
	assert(multilua.luaversion(obj) == 503.0)

	assert(obj:close())
end

-- Test: luaversion meta
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.luaversion) == 'function')
	assert(obj:luaversion() == 503.0)

	assert(obj:close())
end

-- Test: xmove
do
	assert(type(multilua.xmove) == 'function')

	local obj_a = multilua.new()
	local obj_b = multilua.new()

	multilua.dostring(obj_a, "return function() end")
	multilua.xmove(obj_a, obj_b, 1)

	assert(obj_b[-1] == 'function')

	assert(obj_b:close())
	assert(obj_a:close())
end

-- Test: xmove meta
do
	assert(type(multilua.xmove) == 'function')

	local obj_a = multilua.new()
	local obj_b = multilua.new()

	obj_a:dostring("return function() end")
	obj_a:xmove(obj_b, 1)

	assert(obj_b[-1] == 'function')

	assert(obj_b:close())
	assert(obj_a:close())
end

-- TODO: Test: yield
do
	assert(type(multilua.yield) == 'function')
end

-- TODO: Test: yield meta
do
	assert(type(multilua.yield) == 'function')
end

-- Test: getglobal
do
	assert(type(multilua.getglobal) == 'function')

	local obj = assert(multilua.new())
	multilua.pushinteger(obj, 10)
	multilua.setglobal(obj, "hello")

	multilua.getglobal(obj, "hello")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 10)

	assert(obj:close())
end

-- Test: getglobal meta
do
	assert(type(multilua.getglobal) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.getglobal) == 'function')

	obj:pushinteger(10)
	obj:setglobal("hello")

	obj:getglobal("hello")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 10)

	assert(obj:close())
end

-- Test: geti
do
	assert(type(multilua.geti) == 'function')

	local obj = assert(multilua.new())
	multilua.dostring(obj, "return {'a', 'b', 'c'}")

	assert(multilua.geti(obj, -1, 2) == 'string')
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'b')

	assert(obj:close())
end

-- Test: geti meta
do
	assert(type(multilua.geti) == 'function')

	local obj = assert(multilua.new())
	obj:dostring("return {'a', 'b', 'c'}")

	assert(obj:geti(-1, 2) == 'string')
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'b')

	assert(obj:close())
end

-- TODO: Test: getmetatable
do
	assert(type(multilua.getmetatable) == 'function')
end

-- TODO: Test: getmetatable meta
do
	assert(type(multilua.getmetatable) == 'function')
end

-- Test: gettable
do
	assert(type(multilua.gettable) == 'function')

	local obj = assert(multilua.new())

	multilua.dostring(obj, "return {hello='world'}")
	multilua.pushstring(obj, "hello")

	assert(multilua.gettable(obj, -2) == 'string')
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'world')

	assert(obj:close())
end

-- Test: gettable meta
do
	assert(type(multilua.gettable) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.gettable) == 'function')

	obj:dostring("return {hello='world'}")
	obj:pushstring("hello")

	assert(obj:gettable(-2) == 'string')
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'world')

	assert(obj:close())
end

-- TODO: Test: getuservalue
do
	assert(type(multilua.getuservalue) == 'function')
end

-- TODO: Test: getuservalue meta
do
	assert(type(multilua.getuservalue) == 'function')
end

-- Test: insert
do
	assert(type(multilua.insert) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 1))
	assert(multilua.pushinteger(obj, 2))
	assert(multilua.pushinteger(obj, 3))

	assert(multilua.insert(obj, -2))

	assert(obj(-1) == 2)
	assert(obj(-2) == 3)
	assert(obj(-3) == 1)

	assert(obj:close())
end

-- Test: insert meta
do
	assert(type(multilua.insert) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.insert) == 'function')

	assert(obj:pushinteger(1))
	assert(obj:pushinteger(2))
	assert(obj:pushinteger(3))

	assert(obj:insert(-2))

	assert(obj(-1) == 2)
	assert(obj(-2) == 3)
	assert(obj(-3) == 1)

	assert(obj:close())
end

-- Test: isboolean
do
	assert(type(multilua.isboolean) == 'function')

	local obj = assert(multilua.new())
	multilua.pushboolean(obj, false)
	assert(multilua.isboolean(obj, -1))
	multilua.pushboolean(obj, true)
	assert(multilua.isboolean(obj, -1))

	assert(obj:close())
end

-- Test: isboolean meta
do
	assert(type(multilua.isboolean) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.isboolean) == 'function')

	obj:pushboolean(false)
	assert(obj:isboolean(-1))
	obj:pushboolean(true)
	assert(obj:isboolean(-1))

	assert(obj:close())
end

-- Test: iscfunction
do
	assert(type(multilua.iscfunction) == 'function')

	local obj = assert(multilua.new())

	multilua.pushcfunction(obj, multilua.new)
	assert(multilua.iscfunction(obj, -1))

	multilua.pushinteger(obj, 10)
	assert(multilua.iscfunction(obj, -1) == false)

	assert(obj:close())
end

-- Test: iscfunction meta
do
	assert(type(multilua.iscfunction) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.iscfunction) == 'function')

	obj:pushcfunction(multilua.new)
	assert(obj:iscfunction(-1))

	obj:pushinteger(10)
	assert(obj:iscfunction(-1) == false)

	assert(obj:close())
end

-- Test: isfunction
do
	assert(type(multilua.isfunction) == 'function')

	local obj = assert(multilua.new())

	multilua.dostring(obj, "return function() end")
	assert(multilua.isfunction(obj, -1))

	assert(obj:close())
end

-- Test: isfunction meta
do
	assert(type(multilua.isfunction) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.isfunction) == "function")

	obj:dostring("return function() end")
	assert(obj:isfunction(-1))

	assert(obj:close())
end

-- Test: isinteger
do
	assert(type(multilua.isinteger) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	assert(multilua.isinteger(obj, -1))

	multilua.pushnumber(obj, 10)
	assert(multilua.isinteger(obj, -1) == false)

	assert(obj:close())
end

-- Test: isinteger meta
do
	assert(type(multilua.isinteger) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.isinteger) == 'function')

	obj:pushinteger(10)
	assert(obj:isinteger(-1))

	obj:pushnumber(10)
	assert(obj:isinteger(-1) == false)

	assert(obj:close())
end

-- Test: islightuserdata
do
	assert(type(multilua.islightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushlightuserdata(obj, obj.self))

	assert(multilua.islightuserdata(obj, -1))

	assert(obj:close())
end

-- Test: islightuserdata meta
do
	assert(type(multilua.islightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.islightuserdata) == 'function')

	assert(obj:pushlightuserdata(obj.self))

	assert(obj:islightuserdata(-1))

	assert(obj:close())
end

-- Test: isnil
do
	assert(type(multilua.isnil) == 'function')

	local obj = assert(multilua.new())
	multilua.pushnil(obj)
	assert(multilua.isnil(obj, -1))

	assert(obj:close())
end

-- Test: isnil meta
do
	assert(type(multilua.isnil) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isnil) == 'function')

	obj:pushnil()
	assert(obj:isnil(-1))

	assert(obj:close())
end

-- Test: isnone
do
	assert(type(multilua.isnone) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.isnone(obj, 1000))
	assert(multilua.isnone(obj, -1) == false)

	assert(obj:close())
end

-- Test: isnone meta
do
	assert(type(multilua.isnone) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isnone) == 'function')

	assert(obj:isnone(1000))
	assert(obj:isnone(-1) == false)

	assert(obj:close())
end

-- Test: isnoneornil
do
	assert(type(multilua.isnoneornil) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.isnoneornil(obj, 1000))
	assert(multilua.isnoneornil(obj, -1))

	assert(obj:close())
end

-- Test: isnoneornil meta
do
	assert(type(multilua.isnoneornil) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isnoneornil) == 'function')

	assert(obj:isnoneornil(1000))
	assert(obj:isnoneornil(-1))

	assert(obj:close())
end

-- Test: isnumber
do
	assert(type(multilua.isnumber) == 'function')

	local obj = assert(multilua.new())

	multilua.pushnumber(obj, 10)
	assert(multilua.isnumber(obj, -1))

	multilua.pushinteger(obj, 10)
	assert(multilua.isnumber(obj, -1))

	assert(obj:close())
end

-- Test: isnumber meta
do
	assert(type(multilua.isnumber) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isnumber) == 'function')

	obj:pushnumber(10)
	assert(obj:isnumber(-1))

	obj:pushinteger(10)
	assert(obj:isnumber(-1))

	assert(obj:close())
end

-- Test: isstring
do
	assert(type(multilua.isstring) == 'function')

	local obj = assert(multilua.new())

	multilua.pushstring(obj, "Hello, World!")
	assert(multilua.isstring(obj, -1))

	assert(obj:close())
end

-- Test: isstring meta
do
	assert(type(multilua.isstring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isstring) == 'function')

	obj:pushstring("Hello, World!")
	assert(obj:isstring(-1))

	assert(obj:close())
end

-- Test: istable
do
	assert(type(multilua.istable) == 'function')

	local obj = assert(multilua.new())

	multilua.newtable(obj)
	assert(multilua.istable(obj, -1))

	assert(obj:close())
end

-- Test: istable meta
do
	assert(type(multilua.istable) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.istable) == 'function')

	obj:newtable()
	assert(obj:istable(-1))

	assert(obj:close())
end

-- TODO: Test: isthread
do
	assert(type(multilua.isthread) == 'function')
end

-- TODO: Test: isthread meta
do
	assert(type(multilua.isthread) == 'function')
end

-- Test: isuserdata
do
	assert(type(multilua.isuserdata) == 'function')

	local obj = assert(multilua.new())

	multilua.pushlightuserdata(obj, obj.self)
	assert(multilua.isuserdata(obj, -1))

	assert(obj:close())
end

-- Test: isuserdata meta
do
	assert(type(multilua.isuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isuserdata) == 'function')

	obj:pushlightuserdata(obj.self)
	assert(obj:isuserdata(-1))

	assert(obj:close())
end

-- TODO: Test: isyieldable
do
	assert(type(multilua.isyieldable) == 'function')
end

-- TODO: Test: isyieldable meta
do
	assert(type(multilua.isyieldable) == 'function')
end

-- Test: len
do
	assert(type(multilua.len) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.pushstring(obj, "Hello, World!"))
	assert(multilua.len(obj, -1))

	assert(obj(-1) == 13)

	assert(obj:close())
end

-- Test: len meta
do
	assert(type(multilua.len) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.len) == 'function')

	assert(obj:pushstring("Hello, World!"))
	assert(obj:len(-1))

	assert(obj(-1) == 13)

	assert(obj:close())
end

-- Test: newtable
do
	assert(type(multilua.newtable) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.newtable(obj))
	assert(obj[-1] == 'table')

	assert(obj:close())
end

-- Test: newtable meta
do
	assert(type(multilua.newtable) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.newtable) == 'function')

	assert(obj:newtable())
	assert(obj[-1] == 'table')

	assert(obj:close())
end

-- TODO: Test: newthread
do
	assert(type(multilua.newthread) == 'function')
end

-- TODO: Test: newthread meta
do
	assert(type(multilua.newthread) == 'function')
end

-- Test: newuserdata
do
	assert(type(multilua.newuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(multilua.newuserdata(obj, 100)) == 'userdata')

	assert(obj:close())
end

-- Test: newuserdata meta
do
	assert(type(multilua.newuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.newuserdata) == 'function')

	assert(type(obj:newuserdata(100)) == 'userdata')

	assert(obj:close())
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

-- Test: pop
do
	assert(type(multilua.pop) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushstring(obj, "Hi"))
	assert(multilua.pushboolean(obj, true))
	assert(multilua.pushinteger(obj, 10))

	assert(multilua.pop(obj, 2))
	assert(obj[-1] == 'string')

	assert(obj:close())
end

-- Test: pop meta
do
	assert(type(multilua.pop) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pop) == 'function')

	assert(obj:pushstring("Hi"))
	assert(obj:pushboolean(true))
	assert(obj:pushinteger(10))

	assert(obj:pop(2))
	assert(obj[-1] == 'string')

	assert(obj:close())
end

-- Test: pushboolean
do
	assert(type(multilua.pushboolean) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushboolean(obj, true))
	assert(obj[-1] == 'boolean')
	assert(obj(-1) == true)

	assert(multilua.pushboolean(obj, false))
	assert(obj[-1] == 'boolean')
	assert(obj(-1) == false)

	assert(obj:close())
end

-- Test: pushboolean meta
do
	assert(type(multilua.pushboolean) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushboolean) == 'function')

	assert(obj:pushboolean(true))
	assert(obj[-1] == 'boolean')
	assert(obj(-1) == true)

	assert(obj:pushboolean(false))
	assert(obj[-1] == 'boolean')
	assert(obj(-1) == false)

	assert(obj:close())
end

-- Test: pushglobaltable
do
	assert(type(multilua.pushglobaltable) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushglobaltable(obj))
	assert(obj[-1] == 'table')

	assert(obj:close())
end

-- Test: pushglobaltable meta
do
	assert(type(multilua.pushglobaltable) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushglobaltable) == 'function')

	assert(obj:pushglobaltable())
	assert(obj[-1] == 'table')

	assert(obj:close())
end

-- Test: pushinteger
do
	assert(type(multilua.pushinteger) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 10))
	assert(obj[-1] == 'number')
	assert(multilua.isinteger(obj, -1))

	assert(multilua.pushinteger(obj, "Nope") == nil)

	assert(obj:close())
end

-- Test: pushinteger meta
do
	assert(type(multilua.pushinteger) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushinteger) == 'function')

	assert(obj:pushinteger(10))
	assert(obj[-1] == 'number')
	assert(obj:isinteger(-1))

	assert(obj:pushinteger("Nope") == nil)

	assert(obj:close())
end

-- Test: pushstring
do
	assert(type(multilua.pushstring) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushstring(obj, "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello"))
	assert(obj[-1] == 'string')
	assert(obj(-1) == "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello")

	assert(obj:close())
end

-- Test: pushstring meta
do
	assert(type(multilua.pushstring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushstring) == 'function')

	assert(obj:pushstring("helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello"))
	assert(obj[-1] == 'string')
	assert(obj(-1) == "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello")

	assert(obj:close())
end

-- Test: pushnstring
do
	assert(type(multilua.pushnstring) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushnstring(obj, "Hello, World", 5))
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'Hello')

	assert(multilua.pushnstring(obj, "Hello, World", 300))
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'Hello, World')

	assert(obj:close())
end

-- Test: pushnstring meta
do
	assert(type(multilua.pushnstring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushnstring) == 'function')

	assert(obj:pushnstring("Hello, World", 5))
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'Hello')

	assert(obj:pushnstring("Hello, World", 300))
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'Hello, World')

	assert(obj:close())
end

-- Test: pushnil
do
	assert(type(multilua.pushnil) == 'function')

	local obj = assert(multilua.new())

	multilua.pushnil(obj)
	assert(multilua.isnil(obj, -1))

	assert(obj:close())
end

-- Test: pushnil meta
do
	assert(type(multilua.pushnil) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushnil) == 'function')

	obj:pushnil()
	assert(obj:isnil(-1))

	assert(obj:close())
end

-- Test: pushnumber
do
	assert(type(multilua.pushnumber) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushnumber(obj, 10))
	assert(obj[-1] == 'number')

	assert(multilua.pushnumber(obj, "hi") == nil)

	assert(obj:close())
end

-- Test: pushnumber meta
do
	assert(type(multilua.pushnumber) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushnumber) == 'function')

	assert(obj:pushnumber(10))
	assert(obj[-1] == 'number')

	assert(obj:pushnumber("hi") == nil)

	assert(obj:close())
end

-- Test: pushvalue
do
	assert(type(multilua.pushvalue) == 'function')

	local obj = assert(multilua.new())

	multilua.pushstring(obj, "Hello, World!")
	multilua.pushvalue(obj, -1)

	assert(obj[-1] == 'string')
	assert(obj[-2] == 'string')

	assert(obj:close())
end

-- Test: pushvalue meta
do
	assert(type(multilua.pushvalue) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushvalue) == 'function')

	obj:pushstring("Hello, World!")
	obj:pushvalue(-1)

	assert(obj[-1] == 'string')
	assert(obj[-2] == 'string')

	assert(obj:close())
end

-- Test: pushlightuserdata
do
	assert(type(multilua.pushlightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushlightuserdata(obj, obj.self))

	assert(obj:close())
end

-- Test: pushlightuserdata meta
do
	assert(type(multilua.pushlightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushlightuserdata) == 'function')

	assert(obj:pushlightuserdata(obj.self))

	assert(obj:close())
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

-- Test: remove
do
	assert(type(multilua.remove) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 1))
	assert(multilua.pushstring(obj, "Hello"))
	assert(multilua.pushboolean(obj, true))

	assert(multilua.remove(obj, 2))

	assert(obj[-1] == 'boolean')
	assert(obj[-2] == 'number')
	assert(obj[-3] == 'nil')

	assert(obj:close())
end

-- Test: remove meta
do
	assert(type(multilua.remove) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.remove) == 'function')

	assert(obj:pushinteger(1))
	assert(obj:pushstring("Hello"))
	assert(obj:pushboolean(true))

	assert(obj:remove(2))

	assert(obj[-1] == 'boolean')
	assert(obj[-2] == 'number')
	assert(obj[-3] == 'nil')

	assert(obj:close())
end

-- Test: replace
do
	assert(type(multilua.replace) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 1))
	assert(multilua.pushstring(obj, "Hello"))
	assert(multilua.pushboolean(obj, true))

	assert(multilua.replace(obj, 3))

	assert(obj[-1] == 'string')
	assert(obj[-2] == 'number')
	assert(obj[-3] == 'nil')

	assert(obj:close())
end

-- Test: replace meta
do
	assert(type(multilua.replace) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.replace) == 'function')

	assert(obj:pushinteger(1))
	assert(obj:pushstring("Hello"))
	assert(obj:pushboolean(true))

	assert(obj:replace(3))

	assert(obj[-1] == 'string')
	assert(obj[-2] == 'number')
	assert(obj[-3] == 'nil')

	assert(obj:close())
end

-- Test: rotate
do
	assert(type(multilua.rotate) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 1))
	assert(multilua.pushstring(obj, "Hello"))
	assert(multilua.pushboolean(obj, true))

	assert(multilua.rotate(obj, 1, -1))

	assert(obj[-1] == 'number')
	assert(obj[-2] == 'boolean')
	assert(obj[-3] == 'string')

	assert(obj:close())
end

-- Test: rotate meta
do
	assert(type(multilua.rotate) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.rotate) == 'function')

	assert(obj:pushinteger(1))
	assert(obj:pushstring("Hello"))
	assert(obj:pushboolean(true))

	assert(obj:rotate(1, -1))

	assert(obj[-1] == 'number')
	assert(obj[-2] == 'boolean')
	assert(obj[-3] == 'string')

	assert(obj:close())
end

-- Test: setfield
do
	assert(type(multilua.setfield) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.newtable(obj))
	assert(multilua.pushinteger(obj, 10))
	assert(multilua.setfield(obj, -2, "hello"))

	assert(multilua.getfield(obj, -1, "hello"))
	assert(obj(-1) == 10)

	assert(obj:close())
end

-- Test: setfield meta
do
	assert(type(multilua.setfield) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.setfield) == 'function')

	assert(obj:newtable())
	assert(obj:pushinteger(10))
	assert(obj:setfield(-2, "hello"))

	assert(obj:getfield(-1, "hello"))
	assert(obj(-1) == 10)

	assert(obj:close())
end

-- Test: setglobal
do
	assert(type(multilua.setglobal) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 10))
	assert(multilua.setglobal(obj, "nim"))

	assert(multilua.getglobal(obj, "nim"))
	assert(obj(-1) == 10)

	assert(obj:close())
end

-- Test: setglobal meta
do
	assert(type(multilua.setglobal) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.setglobal) == 'function')

	assert(obj:pushinteger(10))
	assert(obj:setglobal("nim"))

	assert(obj:getglobal("nim"))
	assert(obj(-1) == 10)

	assert(obj:close())
end

-- Test: seti
do
	assert(type(multilua.seti) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.newtable(obj))

	assert(multilua.pushstring(obj, "Hi"))
	assert(multilua.seti(obj, -2, 1))

	assert(multilua.pushstring(obj, "Overwrite"))
	assert(multilua.seti(obj, -2, 1))

	assert(multilua.pushstring(obj, "Other"))
	assert(multilua.seti(obj, -2, 2))

	assert(multilua.geti(obj, -1, 1) == 'string')
	assert(obj(-1) == 'Overwrite')
	assert(multilua.pop(obj, 1))

	assert(multilua.geti(obj, -1, 2) == 'string')
	assert(obj(-1) == 'Other')
	assert(multilua.pop(obj, 1))

	assert(obj:close())
end

-- Test: seti meta
do
	assert(type(multilua.seti) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.seti) == 'function')

	assert(obj:newtable())

	assert(obj:pushstring("Hi"))
	assert(obj:seti(-2, 1))

	assert(obj:pushstring("Overwrite"))
	assert(obj:seti(-2, 1))

	assert(obj:pushstring("Other"))
	assert(obj:seti(-2, 2))

	assert(obj:geti(-1, 1) == 'string')
	assert(obj(-1) == 'Overwrite')
	assert(obj:pop(1))

	assert(obj:geti(-1, 2) == 'string')
	assert(obj(-1) == 'Other')
	assert(obj:pop(1))

	assert(obj:close())
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

-- Test: settop
do
	assert(type(multilua.settop) == 'function')

	local obj = assert(multilua.new())

	for i=1, 10 do
		assert(multilua.pushinteger(obj, i))
	end
	assert(multilua.gettop(obj) == 10)

	assert(multilua.settop(obj, 0))
	assert(multilua.gettop(obj) == 0)

	assert(obj:close())
end

-- Test: settop meta
do
	assert(type(multilua.settop) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.settop) == 'function')

	for i=1, 10 do
		assert(obj:pushinteger(i))
	end
	assert(obj:gettop() == 10)

	assert(obj:settop(0))
	assert(obj:gettop() == 0)

	assert(obj:close())
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
	local obj = assert(multilua.new())
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
	local obj = assert(multilua.new())
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

	local obj = assert(multilua.new())

	assert(multilua.pushcfunction(obj, multilua.new))
	assert(multilua.iscfunction(obj, -1))
end

-- Test: pushcfunction meta
do
	assert(type(multilua.pushcfunction) == 'function')

	local obj = assert(multilua.new())

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

-- Test: maxinteger/mininteger
do
	assert(type(multilua.maxinteger) == 'function')
	assert(type(multilua.mininteger) == 'function')

	assert(type(multilua.maxinteger()) == 'number')
	assert(type(multilua.mininteger()) == 'number')
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
