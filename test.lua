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
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	-- Test: self has a metatable
	local t = assert(getmetatable(obj))
	assert(t['__gc'])

	-- Test: closing
	obj = nil
	collectgarbage('collect')
end

-- Test: creation meta
do
	assert(type(multilua.new) == 'function')
	local obj = multilua()
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	-- Test: self has a metatable
	assert(getmetatable(obj))

	-- Test: closing
	obj = nil
	collectgarbage('collect')
end

-- Test: Manual closing
do
	assert(type(multilua.new) == 'function')
	local obj = multilua()
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	-- First close
	assert(multilua.close(obj))
	assert(type(obj.self) == 'nil')

	-- Second close
	assert(multilua.close(obj) == nil)
	assert(type(obj.self) == 'nil')
end

-- Test: Manual closing meta
do
	assert(type(multilua.new) == 'function')
	local obj = multilua()
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	-- First close
	assert(obj:close())
	assert(type(obj.self) == 'nil')

	-- Second close
	assert(obj:close() == nil)
	assert(type(obj.self) == 'nil')
end

-- Test multiple states can exist
-- simultaneously
do
	local obj1 = multilua.new()
	local obj2 = multilua.new()

	assert(obj1.self ~= obj2.self)
end

-- Test equivalents
do
	local obj1 = multilua.new()
	local obj2 = multilua.new()

	assert(obj1 == obj1)
	assert(obj1 ~= obj2)
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
end

-- Test tostring name
do
	local obj = assert(multilua.new())
	assert(tostring(obj):sub(1, 9) == "multilua:")

	
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

	
end

-- Test: openbase
do
	assert(type(multilua.openbase) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openbase(obj))
	assert(obj[-1] == 'function')
end

-- Test: openbase meta
do
	assert(type(multilua.openbase) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openbase) == 'function')

	assert(obj:openbase())
	assert(obj[-1] == 'function')
end

-- Test: openpackage
do
	assert(type(multilua.openpackage) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openpackage(obj))
	assert(obj[-1] == 'function')
end

-- Test: openpackage meta
do
	assert(type(multilua.openpackage) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openpackage) == 'function')

	assert(obj:openpackage())
	assert(obj[-1] == 'function')
end

-- Test: opencoroutine
do
	assert(type(multilua.opencoroutine) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.opencoroutine(obj))
	assert(obj[-1] == 'function')
end

-- Test: opencoroutine meta
do
	assert(type(multilua.opencoroutine) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.opencoroutine) == 'function')

	assert(obj:opencoroutine())
	assert(obj[-1] == 'function')
end

-- Test: openio
do
	assert(type(multilua.openio) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openio(obj))
	assert(obj[-1] == 'function')
end

-- Test: openio meta
do
	assert(type(multilua.openio) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openio) == 'function')

	assert(obj:openio())
	assert(obj[-1] == 'function')
end

-- Test: openos
do
	assert(type(multilua.openos) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openos(obj))
	assert(obj[-1] == 'function')
end

-- Test: openos meta
do
	assert(type(multilua.openos) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openos) == 'function')

	assert(obj:openos())
	assert(obj[-1] == 'function')
end

-- Test: openstring
do
	assert(type(multilua.openstring) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openstring(obj))
	assert(obj[-1] == 'function')
end

-- Test: openstring meta
do
	assert(type(multilua.openstring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openstring) == 'function')

	assert(obj:openstring())
	assert(obj[-1] == 'function')
end

-- Test: openmath
do
	assert(type(multilua.openmath) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openmath(obj))
	assert(obj[-1] == 'function')
end

-- Test: openmath meta
do
	assert(type(multilua.openmath) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openmath) == 'function')

	assert(obj:openmath())
	assert(obj[-1] == 'function')
end

-- Test: openutf8
do
	assert(type(multilua.openutf8) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.openutf8(obj))
	assert(obj[-1] == 'function')
end

-- Test: openutf8 meta
do
	assert(type(multilua.openutf8) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openutf8) == 'function')

	assert(obj:openutf8())
	assert(obj[-1] == 'function')
end

-- Test: opendebug
do
	assert(type(multilua.opendebug) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.opendebug(obj))
	assert(obj[-1] == 'function')
end

-- Test: opendebug meta
do
	assert(type(multilua.opendebug) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.opendebug) == 'function')

	assert(obj:opendebug())
	assert(obj[-1] == 'function')
end

-- Test: bitlib
do
	assert(type(multilua.bitlib) == 'function')

	local obj = assert(multilua.new())

	assert(type(multilua.bitlib(obj)) == 'boolean')
end

-- Test: bitlib meta
do
	assert(type(multilua.bitlib) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.bitlib) == 'function')

	assert(type(obj:bitlib()) == 'boolean')
end

-- Test: openbit32
do
	assert(type(multilua.openbit32) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.openbit32(obj))

	-- bit32 is conditional:
	if multilua.bitlib(obj) then
		assert(obj[-1] == 'function')
	else
		assert(obj[-1] == 'nil')
	end
end

-- Test: openbit32 meta
do
	assert(type(multilua.openbit32) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.openbit32) == 'function')

	assert(obj:openbit32())

	-- bit32 is conditional:
	if multilua.bitlib(obj) then
		assert(obj[-1] == 'function')
	else
		assert(obj[-1] == 'nil')
	end
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

	
end

-- Test: gettop
do
	assert(type(multilua.gettop) == 'function')

	local obj = assert(multilua.new())

	for i = 1, 10 do
		multilua.pushinteger(obj, i)
	end

	assert(multilua.gettop(obj) == 10)

	
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

	
end

-- Test: #
do
	assert(type(multilua.gettop) == 'function')

	local obj = assert(multilua.new())

	for i = 1, 10 do
		multilua.pushinteger(obj, i)
	end

	assert(#obj == 10)

	
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
	assert(multilua.arith(obj, "<") == 40.0)

	-- Bitwise Rightshift
	multilua.pushinteger(obj, 5)
	multilua.pushinteger(obj, 3)
	assert(multilua.arith(obj, ">") == 0.0)

	-- Bitwise NOT
	multilua.pushinteger(obj, -2)
	assert(multilua.arith(obj, "~") == 1.0)

	-- Bitwise XOR
	multilua.pushinteger(obj, 5)
	multilua.pushinteger(obj, 3)
	assert(multilua.arith(obj, "~|") == 6.0)

	
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

	
end

-- Test: checkstack
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.checkstack) == 'function')
	assert(multilua.checkstack(obj, 2) == true)

	
end

-- Test: checkstack meta
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.checkstack) == 'function')
	assert(obj:checkstack(2) == true)

	
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

	
end

-- Test: copy
do
	assert(type(multilua.copy) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 3)

	assert(multilua.copy(obj, -1, -2))
	assert(multilua.tointeger(obj, -2) == 3)

	
end

-- Test: copy meta
do
	assert(type(multilua.copy) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.copy) == 'function')

	obj:pushinteger(3)

	assert(obj:copy(-1, -2))
	assert(obj:tointeger(-2) == 3)

	
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

	
end

-- Test: luaversion
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.luaversion) == 'function')
	assert(multilua.luaversion(obj) == 503.0)

	
end

-- Test: luaversion meta
do
	local obj = assert(multilua.new())
	assert(type(obj) == 'userdata')
	assert(type(obj.self) == 'userdata')

	assert(type(multilua.luaversion) == 'function')
	assert(obj:luaversion() == 503.0)

	
end

-- Test: xmove
do
	assert(type(multilua.xmove) == 'function')

	local obj_a = multilua.new()
	local obj_b = multilua.new()

	multilua.dostring(obj_a, "return function() end")
	multilua.xmove(obj_a, obj_b, 1)

	assert(obj_b[-1] == 'function')
end

-- Test: xmove meta
do
	assert(type(multilua.xmove) == 'function')

	local obj_a = multilua.new()
	local obj_b = multilua.new()

	obj_a:dostring("return function() end")
	obj_a:xmove(obj_b, 1)

	assert(obj_b[-1] == 'function')
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

	
end

-- Test: geti
do
	assert(type(multilua.geti) == 'function')

	local obj = assert(multilua.new())
	multilua.dostring(obj, "return {'a', 'b', 'c'}")

	assert(multilua.geti(obj, -1, 2) == 'string')
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'b')

	
end

-- Test: geti meta
do
	assert(type(multilua.geti) == 'function')

	local obj = assert(multilua.new())
	obj:dostring("return {'a', 'b', 'c'}")

	assert(obj:geti(-1, 2) == 'string')
	assert(obj[-1] == 'string')
	assert(obj(-1) == 'b')

	
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

	
end

-- Test: isboolean
do
	assert(type(multilua.isboolean) == 'function')

	local obj = assert(multilua.new())
	multilua.pushboolean(obj, false)
	assert(multilua.isboolean(obj, -1))
	multilua.pushboolean(obj, true)
	assert(multilua.isboolean(obj, -1))

	
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

	
end

-- Test: iscfunction
do
	assert(type(multilua.iscfunction) == 'function')

	local obj = assert(multilua.new())

	multilua.pushcfunction(obj, multilua.new)
	assert(multilua.iscfunction(obj, -1))

	multilua.pushinteger(obj, 10)
	assert(multilua.iscfunction(obj, -1) == false)

	
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

	
end

-- Test: isfunction
do
	assert(type(multilua.isfunction) == 'function')

	local obj = assert(multilua.new())

	multilua.dostring(obj, "return function() end")
	assert(multilua.isfunction(obj, -1))

	
end

-- Test: isfunction meta
do
	assert(type(multilua.isfunction) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.isfunction) == "function")

	obj:dostring("return function() end")
	assert(obj:isfunction(-1))

	
end

-- Test: isinteger
do
	assert(type(multilua.isinteger) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	assert(multilua.isinteger(obj, -1))

	multilua.pushnumber(obj, 10)
	assert(multilua.isinteger(obj, -1) == false)

	
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

	
end

-- Test: islightuserdata
do
	assert(type(multilua.islightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushlightuserdata(obj, obj.self))

	assert(multilua.islightuserdata(obj, -1))

	
end

-- Test: islightuserdata meta
do
	assert(type(multilua.islightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.islightuserdata) == 'function')

	assert(obj:pushlightuserdata(obj.self))

	assert(obj:islightuserdata(-1))

	
end

-- Test: isnil
do
	assert(type(multilua.isnil) == 'function')

	local obj = assert(multilua.new())
	multilua.pushnil(obj)
	assert(multilua.isnil(obj, -1))

	
end

-- Test: isnil meta
do
	assert(type(multilua.isnil) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isnil) == 'function')

	obj:pushnil()
	assert(obj:isnil(-1))

	
end

-- Test: isnone
do
	assert(type(multilua.isnone) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.isnone(obj, 1000))
	assert(multilua.isnone(obj, -1) == false)

	
end

-- Test: isnone meta
do
	assert(type(multilua.isnone) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isnone) == 'function')

	assert(obj:isnone(1000))
	assert(obj:isnone(-1) == false)

	
end

-- Test: isnoneornil
do
	assert(type(multilua.isnoneornil) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.isnoneornil(obj, 1000))
	assert(multilua.isnoneornil(obj, -1))

	
end

-- Test: isnoneornil meta
do
	assert(type(multilua.isnoneornil) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isnoneornil) == 'function')

	assert(obj:isnoneornil(1000))
	assert(obj:isnoneornil(-1))

	
end

-- Test: isnumber
do
	assert(type(multilua.isnumber) == 'function')

	local obj = assert(multilua.new())

	multilua.pushnumber(obj, 10)
	assert(multilua.isnumber(obj, -1))

	multilua.pushinteger(obj, 10)
	assert(multilua.isnumber(obj, -1))

	
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

	
end

-- Test: isstring
do
	assert(type(multilua.isstring) == 'function')

	local obj = assert(multilua.new())

	multilua.pushstring(obj, "Hello, World!")
	assert(multilua.isstring(obj, -1))

	
end

-- Test: isstring meta
do
	assert(type(multilua.isstring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isstring) == 'function')

	obj:pushstring("Hello, World!")
	assert(obj:isstring(-1))

	
end

-- Test: istable
do
	assert(type(multilua.istable) == 'function')

	local obj = assert(multilua.new())

	multilua.newtable(obj)
	assert(multilua.istable(obj, -1))

	
end

-- Test: istable meta
do
	assert(type(multilua.istable) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.istable) == 'function')

	obj:newtable()
	assert(obj:istable(-1))

	
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

	
end

-- Test: isuserdata meta
do
	assert(type(multilua.isuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.isuserdata) == 'function')

	obj:pushlightuserdata(obj.self)
	assert(obj:isuserdata(-1))

	
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

	
end

-- Test: len meta
do
	assert(type(multilua.len) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.len) == 'function')

	assert(obj:pushstring("Hello, World!"))
	assert(obj:len(-1))

	assert(obj(-1) == 13)

	
end

-- Test: newtable
do
	assert(type(multilua.newtable) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.newtable(obj))
	assert(obj[-1] == 'table')

	
end

-- Test: newtable meta
do
	assert(type(multilua.newtable) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.newtable) == 'function')

	assert(obj:newtable())
	assert(obj[-1] == 'table')

	
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

	
end

-- Test: newuserdata meta
do
	assert(type(multilua.newuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.newuserdata) == 'function')

	assert(type(obj:newuserdata(100)) == 'userdata')

	
end

-- Test: next
do
	assert(type(multilua.next) == 'function')

	local obj = assert(multilua.new())

	-- Empty table
	multilua.newtable(obj)
	multilua.pushnil(obj)
	assert(multilua.next(obj, -2) == 0)
	assert(obj[-1] == 'table')
	assert(obj[-2] == 'nil')
	assert(obj[-3] == 'boolean')

	-- array
	multilua.dostring(obj, "return {'a', 'b', 'c'}")
	multilua.pushnil(obj)
	assert(multilua.next(obj, -2) == 1)
	assert(obj[-1] == 'string')
	assert(obj[-2] == 'number')
	assert(obj[-3] == 'table')

	-- table
	multilua.dostring(obj, "return {hello = function() end}")
	multilua.pushnil(obj)
	assert(multilua.next(obj, -2) == 1)
	assert(obj[-1] == 'function')
	assert(obj[-2] == 'string')
	assert(obj[-3] == 'table')

	-- Demonstration
	multilua.dostring(obj, "return {hello = function() end}")
	multilua.pushnil(obj)
	while multilua.next(obj, -2) ~= 0 do
		-- The value is now at -1
		assert(obj[-1] == 'function')
		-- The key is now at -2
		assert(obj(-2) == 'hello')

		-- Remove the value, keep the key, so iteration continues.
		multilua.pop(obj, 1)
	end
end

-- TODO: Test: next meta
do
	assert(type(multilua.next) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.next) == 'function')

	-- Empty table
	obj:newtable()
	obj:pushnil()
	assert(obj:next(-2) == 0)
	assert(obj[-1] == 'table')
	assert(obj[-2] == 'nil')
	assert(obj[-3] == 'boolean')

	-- array
	obj:dostring("return {'a', 'b', 'c'}")
	obj:pushnil()
	assert(obj:next(-2) == 1)
	assert(obj[-1] == 'string')
	assert(obj[-2] == 'number')
	assert(obj[-3] == 'table')

	-- table
	obj:dostring("return {hello = function() end}")
	obj:pushnil()
	assert(obj:next(-2) == 1)
	assert(obj[-1] == 'function')
	assert(obj[-2] == 'string')
	assert(obj[-3] == 'table')

	-- Demonstration
	obj:dostring("return {hello = function() end}")
	obj:pushnil()
	while obj:next(-2) ~= 0 do
		-- The value is now at -1
		assert(obj[-1] == 'function')
		-- The key is now at -2
		assert(obj(-2) == 'hello')

		-- Remove the value, keep the key, so iteration continues.
		obj:pop(1)
	end
end

-- Test: pcall
do
	assert(type(multilua.pcall) == 'function')

	local obj = assert(multilua.new())

	multilua.dostring(obj, "return function() return error(\"Raises!\") end")
	local status, message = multilua.pcall(obj, 0, nil, 0)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')

	multilua.dostring(obj, "return function() return 2 end")
	status, message = multilua.pcall(obj, 0, nil, 0)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 2)

	multilua.dostring(obj, "return function(a) return 2 + a end")
	multilua.pushinteger(obj, 10)
	status, message = multilua.pcall(obj, 1, nil, 0)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 12)

	multilua.dostring(obj, "return function(a) return error(\"Raises\") end")
	multilua.pushinteger(obj, 10)
	status, message = multilua.pcall(obj, 1, nil, 0)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')

	multilua.dostring(obj, "return function() return error(\"Raises!\") end")
	local status, message = multilua.pcall(obj, 0)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')

	multilua.dostring(obj, "return function() return 2 end")
	status, message = multilua.pcall(obj, 0)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 2)

	multilua.dostring(obj, "return function(a) return 2 + a end")
	multilua.pushinteger(obj, 10)
	status, message = multilua.pcall(obj, 1)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 12)

	multilua.dostring(obj, "return function(a) return error(\"Raises\") end")
	multilua.pushinteger(obj, 10)
	status, message = multilua.pcall(obj, 1)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')
end

-- Test: pcall meta
do
	assert(type(multilua.pcall) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pcall) == 'function')

	obj:dostring("return function() return error(\"Raises!\") end")
	local status, message = obj:pcall(0, nil, 0)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')

	obj:dostring("return function() return 2 end")
	status, message = obj:pcall(0, nil, 0)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 2)

	obj:dostring("return function(a) return 2 + a end")
	obj:pushinteger(10)
	status, message = obj:pcall(1, nil, 0)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 12)

	obj:dostring("return function(a) return error(\"Raises\") end")
	obj:pushinteger(10)
	status, message = obj:pcall(1, nil, 0)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')

	obj:dostring("return function() return error(\"Raises!\") end")
	local status, message = obj:pcall(0)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')

	obj:dostring("return function() return 2 end")
	status, message = obj:pcall(0)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 2)

	obj:dostring("return function(a) return 2 + a end")
	obj:pushinteger(10)
	status, message = obj:pcall(1)
	assert(status == true)
	assert(message == "ok")
	assert(obj[-1] == 'number')
	assert(obj(-1) == 12)

	obj:dostring("return function(a) return error(\"Raises\") end")
	obj:pushinteger(10)
	status, message = obj:pcall(1)
	assert(status == nil)
	assert(message == "runtime")
	assert(obj[-1] == 'string')
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

	
end

-- Test: pushglobaltable
do
	assert(type(multilua.pushglobaltable) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushglobaltable(obj))
	assert(obj[-1] == 'table')

	
end

-- Test: pushglobaltable meta
do
	assert(type(multilua.pushglobaltable) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushglobaltable) == 'function')

	assert(obj:pushglobaltable())
	assert(obj[-1] == 'table')

	
end

-- Test: pushinteger
do
	assert(type(multilua.pushinteger) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 10))
	assert(obj[-1] == 'number')
	assert(multilua.isinteger(obj, -1))

	assert(multilua.pushinteger(obj, "Nope") == nil)

	
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

	
end

-- Test: pushstring
do
	assert(type(multilua.pushstring) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushstring(obj, "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello"))
	assert(obj[-1] == 'string')
	assert(obj(-1) == "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello")

	
end

-- Test: pushstring meta
do
	assert(type(multilua.pushstring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushstring) == 'function')

	assert(obj:pushstring("helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello"))
	assert(obj[-1] == 'string')
	assert(obj(-1) == "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789+hello")

	
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

	
end

-- Test: pushnil
do
	assert(type(multilua.pushnil) == 'function')

	local obj = assert(multilua.new())

	multilua.pushnil(obj)
	assert(multilua.isnil(obj, -1))

	
end

-- Test: pushnil meta
do
	assert(type(multilua.pushnil) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushnil) == 'function')

	obj:pushnil()
	assert(obj:isnil(-1))

	
end

-- Test: pushnumber
do
	assert(type(multilua.pushnumber) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushnumber(obj, 10))
	assert(obj[-1] == 'number')

	assert(multilua.pushnumber(obj, "hi") == nil)

	
end

-- Test: pushnumber meta
do
	assert(type(multilua.pushnumber) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushnumber) == 'function')

	assert(obj:pushnumber(10))
	assert(obj[-1] == 'number')

	assert(obj:pushnumber("hi") == nil)

	
end

-- Test: pushfloat
do
	assert(type(multilua.pushfloat) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushfloat(obj, 10))
	assert(obj[-1] == 'number')

	assert(multilua.pushfloat(obj, "hi") == nil)

	
end

-- Test: pushfloat meta
do
	assert(type(multilua.pushfloat) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushfloat) == 'function')

	assert(obj:pushfloat(10))
	assert(obj[-1] == 'number')

	assert(obj:pushfloat("hi") == nil)

	
end

-- Test: pushvalue
do
	assert(type(multilua.pushvalue) == 'function')

	local obj = assert(multilua.new())

	multilua.pushstring(obj, "Hello, World!")
	multilua.pushvalue(obj, -1)

	assert(obj[-1] == 'string')
	assert(obj[-2] == 'string')

	
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

	
end

-- Test: pushlightuserdata
do
	assert(type(multilua.pushlightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushlightuserdata(obj, obj.self))

	
end

-- Test: pushlightuserdata meta
do
	assert(type(multilua.pushlightuserdata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.pushlightuserdata) == 'function')

	assert(obj:pushlightuserdata(obj.self))

	
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

	
end

-- Test: setglobal
do
	assert(type(multilua.setglobal) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.pushinteger(obj, 10))
	assert(multilua.setglobal(obj, "nim"))

	assert(multilua.getglobal(obj, "nim"))
	assert(obj(-1) == 10)

	
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

	
end

-- Test: setmetatable
do
	assert(type(multilua.setmetatable) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.dostring(obj, "return {'table'}"))
	assert(multilua.dostring(obj, "return {'metatable'}"))
	assert(multilua.setmetatable(obj, -2))

	-- Confirm we have the table, now...
	assert(multilua.pushinteger(obj, 1))
	assert(multilua.gettable(obj, -2))
	assert(obj(-1) == 'table')
	assert(multilua.pop(obj, 1)) -- Clean up

	-- Confirm we can get from the metatable
	assert(multilua.getmetatable(obj, -1))
	assert(multilua.pushinteger(obj, 1))
	assert(multilua.gettable(obj, -2))
	assert(obj(-1) == 'metatable')
end

-- Test: setmetatable meta
do
	assert(type(multilua.setmetatable) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.setmetatable) == 'function')

	assert(obj:dostring("return {'table'}"))
	assert(obj:dostring("return {'metatable'}"))
	assert(obj:setmetatable(-2))

	-- Confirm we have the table, now...
	assert(obj:pushinteger(1))
	assert(obj:gettable(-2))
	assert(obj(-1) == 'table')
	assert(obj:pop(1)) -- Clean up

	-- Confirm we can get from the metatable
	assert(obj:getmetatable(-1))
	assert(obj:pushinteger(1))
	assert(obj:gettable(-2))
	assert(obj(-1) == 'metatable')
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

-- Test: toboolean
do
	assert(type(multilua.toboolean) == 'function')

	local obj = assert(multilua.new())
	multilua.pushboolean(obj, true)

	local x = multilua.toboolean(obj, -1)
	assert(x == true)

	multilua.pushboolean(obj, false)

	x = multilua.toboolean(obj, -1)
	assert(x == false)
end

-- Test: toboolean meta
do
	assert(type(multilua.toboolean) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.toboolean) == 'function')

	obj:pushboolean(true)

	local x = obj:toboolean(-1)
	assert(x == true)

	obj:pushboolean(false)

	x = obj:toboolean(-1)
	assert(x == false)
end

-- Test: tointeger
do
	assert(type(multilua.tointeger) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.pushinteger(obj, 10))

	assert(multilua.tointeger(obj, -1) == 10)

	assert(math.type(multilua.tointeger(obj, -1)) == 'integer')
end

-- Test: tointeger meta
do
	assert(type(multilua.tointeger) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.tointeger) == 'function')

	assert(obj:pushinteger(10))

	assert(obj:tointeger(-1) == 10)

	assert(math.type(obj:tointeger(-1)) == 'integer')
end

-- Test: tointegerx
do
	assert(type(multilua.tointegerx) == 'function')

	local obj = assert(multilua.new())
	assert(multilua.pushinteger(obj, 10))

	assert(multilua.tointegerx(obj, -1) == 10)

	assert(math.type(multilua.tointegerx(obj, -1)) == 'integer')

	multilua.pushstring(obj, "This")
	assert(multilua.tointegerx(obj, -1) == nil)
end

-- Test: tointegerx meta
do
	assert(type(multilua.tointegerx) == 'function')

	local obj = assert(multilua.new())
	assert(type(obj.tointegerx) == 'function')

	assert(obj:pushinteger(10))

	assert(obj:tointegerx(-1) == 10)

	assert(math.type(obj:tointegerx(-1)) == 'integer')

	obj:pushstring("This")
	assert(obj:tointegerx(-1) == nil)
end

-- Test: tostring
do
	assert(type(multilua.tostring) == 'function')

	local obj = assert(multilua.new())

	multilua.pushstring(obj, "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789hello")
	assert(multilua.tostring(obj, -1) == "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789hello")

	multilua.pushinteger(obj, 100)
	assert(multilua.tostring(obj, -1) == "100")

	multilua.pushnumber(obj, 100)
	assert(multilua.tostring(obj, -1) == "100.0")
end

-- Test: tostring meta
do
	assert(type(multilua.tostring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.tostring) == 'function')

	obj:pushstring("helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789hello")
	assert(obj:tostring(-1) == "helloöâîí¬®úøøãöâîí¬®®¯áóäæçèêëì»ñ÷åòôùõõéïðÛÝÜ±²³´µ¶·¸¹°­½0123456789hello")

	obj:pushinteger(100)
	assert(obj:tostring(-1) == "100")

	obj:pushnumber(100)
	assert(obj:tostring(-1) == "100.0")
end

-- Test: tonumber
do
	assert(type(multilua.tonumber) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	assert(multilua.tonumber(obj, -1) == 10.0)
	assert(math.type(multilua.tonumber(obj, -1)) == 'float')

	multilua.pushnumber(obj, 10)
	assert(multilua.tonumber(obj, -1) == 10.0)
	assert(math.type(multilua.tonumber(obj, -1)) == 'float')
end

-- Test: tonumber meta
do
	assert(type(multilua.tonumber) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.tonumber) == 'function')

	obj:pushinteger(10)
	assert(obj:tonumber(-1) == 10.0)
	assert(math.type(obj:tonumber(-1)) == 'float')

	obj:pushnumber(10)
	assert(obj:tonumber(-1) == 10.0)
	assert(math.type(obj:tonumber(-1)) == 'float')
end

-- Test: tonumberx
do
	assert(type(multilua.tonumberx) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	assert(multilua.tonumberx(obj, -1) == 10.0)
	assert(math.type(multilua.tonumberx(obj, -1)) == 'float')

	multilua.pushnumber(obj, 10)
	assert(multilua.tonumberx(obj, -1) == 10.0)
	assert(math.type(multilua.tonumberx(obj, -1)) == 'float')

	multilua.pushstring(obj, "10")
	assert(multilua.tonumberx(obj, -1) == 10.0)
	assert(math.type(multilua.tonumberx(obj, -1)) == 'float')

	multilua.pushstring(obj, "a10")
	assert(multilua.tonumberx(obj, -1) == nil)
end

-- Test: tonumberx meta
do
	assert(type(multilua.tonumberx) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.tonumberx) == 'function')

	obj:pushinteger(10)
	assert(obj:tonumberx(-1) == 10.0)
	assert(math.type(obj:tonumberx(-1)) == 'float')

	obj:pushnumber(10)
	assert(obj:tonumberx(-1) == 10.0)
	assert(math.type(obj:tonumberx(-1)) == 'float')

	obj:pushstring("10")
	assert(obj:tonumberx(-1) == 10.0)
	assert(math.type(obj:tonumberx(-1)) == 'float')

	obj:pushstring("a10")
	assert(obj:tonumberx(-1) == nil)
end

-- Test: tofloat
do
	assert(type(multilua.tofloat) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	assert(multilua.tofloat(obj, -1) == 10.0)
	assert(math.type(multilua.tofloat(obj, -1)) == 'float')

	multilua.pushnumber(obj, 10)
	assert(multilua.tofloat(obj, -1) == 10.0)
	assert(math.type(multilua.tofloat(obj, -1)) == 'float')
end

-- Test: tofloat meta
do
	assert(type(multilua.tofloat) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.tofloat) == 'function')

	obj:pushinteger(10)
	assert(obj:tofloat(-1) == 10.0)
	assert(math.type(obj:tofloat(-1)) == 'float')

	obj:pushnumber(10)
	assert(obj:tofloat(-1) == 10.0)
	assert(math.type(obj:tofloat(-1)) == 'float')
end

-- Test: tofloatx
do
	assert(type(multilua.tofloatx) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	assert(multilua.tofloatx(obj, -1) == 10.0)
	assert(math.type(multilua.tofloatx(obj, -1)) == 'float')

	multilua.pushnumber(obj, 10)
	assert(multilua.tofloatx(obj, -1) == 10.0)
	assert(math.type(multilua.tofloatx(obj, -1)) == 'float')

	multilua.pushstring(obj, "10")
	assert(multilua.tofloatx(obj, -1) == 10.0)
	assert(math.type(multilua.tofloatx(obj, -1)) == 'float')

	multilua.pushstring(obj, "a10")
	assert(multilua.tofloatx(obj, -1) == nil)
end

-- Test: tofloatx meta
do
	assert(type(multilua.tofloatx) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.tofloatx) == 'function')

	obj:pushinteger(10)
	assert(obj:tofloatx(-1) == 10.0)
	assert(math.type(obj:tofloatx(-1)) == 'float')

	obj:pushnumber(10)
	assert(obj:tofloatx(-1) == 10.0)
	assert(math.type(obj:tofloatx(-1)) == 'float')

	obj:pushstring("10")
	assert(obj:tofloatx(-1) == 10.0)
	assert(math.type(obj:tofloatx(-1)) == 'float')

	obj:pushstring("a10")
	assert(obj:tofloatx(-1) == nil)
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

-- Test: type
do
	assert(type(multilua.type) == 'function')

	local obj = assert(multilua.new())

	multilua.pushinteger(obj, 10)
	assert(multilua.type(obj, -1) == 'number')

	multilua.pushnumber(obj, 10)
	assert(multilua.type(obj, -1) == 'number')

	multilua.pushfloat(obj, 10)
	assert(multilua.type(obj, -1) == 'number')

	multilua.pushstring(obj, "Hello")
	assert(multilua.type(obj, -1) == 'string')

	multilua.pushboolean(obj, true)
	assert(multilua.type(obj, -1) == 'boolean')
	multilua.pushboolean(obj, false)
	assert(multilua.type(obj, -1) == 'boolean')

	multilua.pushnil(obj)
	assert(multilua.type(obj, -1) == 'nil')

	multilua.pushcfunction(obj, multilua.new)
	assert(multilua.type(obj, -1) == 'function')

	multilua.dostring(obj, "return function() end")
	assert(multilua.type(obj, -1) == 'function')

	multilua.pushlightuserdata(obj, obj.self)
	assert(multilua.type(obj, -1) == 'userdata')

	multilua.newuserdata(obj, 1)
	assert(multilua.type(obj, -1) == 'userdata')

	multilua.newtable(obj)
	assert(multilua.type(obj, -1) == 'table')
end

-- Test: type meta
do
	assert(type(multilua.type) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.type) == 'function')

	obj:pushinteger(10)
	assert(obj:type(-1) == 'number')

	obj:pushnumber(10)
	assert(obj:type(-1) == 'number')

	obj:pushfloat(10)
	assert(obj:type(-1) == 'number')

	obj:pushstring("Hello")
	assert(obj:type(-1) == 'string')

	obj:pushboolean(true)
	assert(obj:type(-1) == 'boolean')
	obj:pushboolean(false)
	assert(obj:type(-1) == 'boolean')

	obj:pushnil()
	assert(obj:type(-1) == 'nil')

	obj:pushcfunction(multilua.new)
	assert(obj:type(-1) == 'function')

	obj:dostring("return function() end")
	assert(obj:type(-1) == 'function')

	obj:pushlightuserdata(obj.self)
	assert(obj:type(-1) == 'userdata')

	obj:newuserdata(1)
	assert(obj:type(-1) == 'userdata')

	obj:newtable()
	assert(obj:type(-1) == 'table')
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

-- Test: argcheck
do
	assert(type(multilua.argcheck) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: argcheck meta
do
	assert(type(multilua.argcheck) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.argcheck) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: argerror
do
	assert(type(multilua.argerror) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: argerror meta
do
	assert(type(multilua.argerror) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.argerror) == 'function')	

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- TODO: Test: callmeta
do
	assert(type(multilua.callmeta) == 'function')
end

-- TODO: Test: callmeta meta
do
	assert(type(multilua.callmeta) == 'function')
end

-- Test: checkany
do
	assert(type(multilua.checkany) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkany meta
do
	assert(type(multilua.checkany) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checkany) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkinteger
do
	assert(type(multilua.checkinteger) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkinteger meta
do
	assert(type(multilua.checkinteger) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checkinteger) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkstring
do
	assert(type(multilua.checkstring) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkstring meta
do
	assert(type(multilua.checkstring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checkstring) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checknumber
do
	assert(type(multilua.checknumber) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checknumber meta
do
	assert(type(multilua.checknumber) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checknumber) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: lcheckstack
do
	assert(type(multilua.lcheckstack) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: lcheckstack meta
do
	assert(type(multilua.lcheckstack) == 'function')

	local obj = assert(multilua.new())

	assert(type(multilua.lcheckstack) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checktype
do
	assert(type(multilua.checktype) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checktype meta
do
	assert(type(multilua.checktype) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checktype) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkudata
do
	assert(type(multilua.checkudata) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkudata meta
do
	assert(type(multilua.checkudata) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checkudata) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkversion
do
	assert(type(multilua.checkversion) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkversion meta
do
	assert(type(multilua.checkversion) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checkversion) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: dofile
do
	assert(type(multilua.dofile) == 'function')

	local fname = os.tmpname()
	local f = assert(io.open(fname, "w"))
	f:write("return 4")
	f:flush()
	f:close()

	local obj = assert(multilua.new())

	assert(multilua.dofile(obj, fname))
	assert(obj[-1] == 'number')
	assert(obj(-1) == 4)

	os.remove(fname)
end

-- Test: dofile meta
do
	assert(type(multilua.dofile) == 'function')

	local fname = os.tmpname()
	local f = assert(io.open(fname, "w"))
	f:write("return 4")
	f:flush()
	f:close()

	local obj = assert(multilua.new())

	assert(type(obj.dofile) == 'function')

	assert(obj:dofile(fname))
	assert(obj[-1] == 'number')
	assert(obj(-1) == 4)

	os.remove(fname)
end

-- Test: dostring
do
	assert(type(multilua.dostring) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.dostring(obj, "return function() end"))
	assert(multilua.type(obj, -1) == 'function')

	assert(multilua.dostring(obj, "return (function() return 2 end)()"))
	assert(multilua.type(obj, -1) == 'number')
end

-- Test: dostring meta
do
	assert(type(multilua.dostring) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.dostring) == 'function')

	assert(multilua.dostring(obj, "return function() end"))
	assert(multilua.type(obj, -1) == 'function')

	assert(multilua.dostring(obj, "return (function() return 2 end)()"))
	assert(multilua.type(obj, -1) == 'number')
end

-- Test: execresult
do
	assert(type(multilua.execresult) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.execresult(obj, 0) == true)
	assert(obj(-1) == 0)
	assert(obj(-2) == "exit")
	assert(obj(-3) == true)

	--[[
		As this is testing platform specific stuff,
		it cannot be properly tested.
	]]
end

-- Test: execresult meta
do
	assert(type(multilua.execresult) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.execresult) == 'function')

	assert(obj:execresult(0) == true)
	assert(obj(-1) == 0)
	assert(obj(-2) == "exit")
	assert(obj(-3) == true)

	--[[
		As this is testing platform specific stuff,
		it cannot be properly tested.
	]]
end

-- Test: fileresult
do
	assert(type(multilua.fileresult) == 'function')

	local obj = assert(multilua.new())

	assert(multilua.fileresult(obj, 0, "test.lua"))

	--[[
		As this is testing platform specific stuff,
		it cannot be properly tested.
	]]
end

-- Test: fileresult meta
do
	assert(type(multilua.fileresult) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.fileresult) == 'function')

	assert(obj:fileresult(0, "test.lua"))

	--[[
		As this is testing platform specific stuff,
		it cannot be properly tested.
	]]
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

-- Test: registryindex
do
	assert(type(multilua.registryindex) == 'function')

	local obj = assert(multilua.new())
	assert(type(multilua.registryindex(obj)) == 'number')
end

-- Test: registryindex meta
do
	assert(type(multilua.registryindex) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.registryindex) == 'function')

	assert(type(obj:registryindex()) == 'number')
end

-- TODO: Test: ridx_mainthread
do
	assert(type(multilua.ridx_mainthread) == 'function')
end

-- TODO: Test: ridx_mainthread meta
do
	assert(type(multilua.ridx_mainthread) == 'function')
end

-- TODO: Test: ridx_globals
do
	assert(type(multilua.ridx_globals) == 'function')
end

-- TODO: Test: ridx_globals meta
do
	assert(type(multilua.ridx_globals) == 'function')
end

-- Test: ref
do
	assert(type(multilua.ref) == 'function')

	local obj = assert(multilua.new())
	local ref = nil
	local prevref = nil

	assert(multilua.pushinteger(obj, 12))
	ref = multilua.ref(obj, multilua.registryindex(obj))
	assert(type(ref) == 'number')
	assert(ref ~= multilua.noref())
	prevref = ref

	assert(multilua.pushinteger(obj, 12))
	ref = multilua.ref(obj, multilua.registryindex(obj))
	assert(type(ref) == 'number')
	assert(ref ~= multilua.noref())
	assert(prevref < ref)

	assert(multilua.pushnil(obj))
	ref = multilua.ref(obj, multilua.registryindex(obj))
	assert(ref == multilua.refnil())
	assert(ref ~= multilua.noref())
end

-- Test: ref meta
do
	assert(type(multilua.ref) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.ref) == 'function')

	local ref = nil
	local prevref = nil

	assert(obj:pushinteger(12))
	ref = obj:ref(obj:registryindex())
	assert(type(ref) == 'number')
	assert(ref ~= multilua.noref())
	prevref = ref

	assert(obj:pushinteger(12))
	ref = obj:ref(obj:registryindex())
	assert(type(ref) == 'number')
	assert(ref ~= multilua.noref())
	assert(prevref < ref)

	assert(obj:pushnil())
	ref = obj:ref(obj:registryindex())
	assert(ref == obj:refnil())
	assert(ref ~= obj:noref())
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

-- Test: checkoption
do
	assert(type(multilua.checkoption) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: checkoption meta
do
	assert(type(multilua.checkoption) == 'function')

	local obj = assert(multilua.new())

	assert(type(obj.checkoption) == 'function')

	--[[

	This function _cannot_ be tested, as it raises an error
	in non-host Lua, which cannot be caught by pcall.

	]]--
end

-- Test: maxinteger/mininteger
do
	assert(type(multilua.maxinteger) == 'function')
	assert(type(multilua.mininteger) == 'function')

	assert(type(multilua.maxinteger()) == 'number')
	assert(type(multilua.mininteger()) == 'number')
end

-- TODO: Test: version_major
do
	assert(type(multilua.version_major) == 'function')
end

-- TODO: Test: version_major meta
do
	assert(type(multilua.version_major) == 'function')
end

-- TODO: Test: version_minor
do
	assert(type(multilua.version_minor) == 'function')
end

-- TODO: Test: version_minor meta
do
	assert(type(multilua.version_minor) == 'function')
end

-- TODO: Test: version_release
do
	assert(type(multilua.version_release) == 'function')
end

-- TODO: Test: version_release meta
do
	assert(type(multilua.version_release) == 'function')
end

-- TODO: Test: version_string
do
	assert(type(multilua.version_string) == 'function')
end

-- TODO: Test: version_string meta
do
	assert(type(multilua.version_string) == 'function')
end

-- TODO: Test: release_string
do
	assert(type(multilua.release_string) == 'function')
end

-- TODO: Test: release_string meta
do
	assert(type(multilua.release_string) == 'function')
end

-- TODO: Test: luacopyright
do
	assert(type(multilua.luacopyright) == 'function')
end

-- TODO: Test: luacopyright meta
do
	assert(type(multilua.luacopyright) == 'function')
end

-- TODO: Test: luaauthors
do
	assert(type(multilua.luaauthors) == 'function')
end

-- TODO: Test: luaauthors meta
do
	assert(type(multilua.luaauthors) == 'function')
end

-- TODO: Test: signature
do
	assert(type(multilua.signature) == 'function')
end

-- TODO: Test: signature meta
do
	assert(type(multilua.signature) == 'function')
end

-- TODO: Test: multret
do
	assert(type(multilua.multret) == 'function')
end

-- TODO: Test: multret meta
do
	assert(type(multilua.multret) == 'function')
end

-- TODO: Test: minstack
do
	assert(type(multilua.minstack) == 'function')
end

-- TODO: Test: minstack meta
do
	assert(type(multilua.minstack) == 'function')
end

-- TODO: Test: ridx_last
do
	assert(type(multilua.ridx_last) == 'function')
end

-- TODO: Test: ridx_last meta
do
	assert(type(multilua.ridx_last) == 'function')
end

-- TODO: Test: user_header
do
	assert(type(multilua.user_header) == 'function')
end

-- TODO: Test: user_header meta
do
	assert(type(multilua.user_header) == 'function')
end

-- TODO: Test: upvalueindex
do
	assert(type(multilua.upvalueindex) == 'function')
end

-- TODO: Test: upvalueindex meta
do
	assert(type(multilua.upvalueindex) == 'function')
end

-- TODO: Test: hookcall
do
	assert(type(multilua.hookcall) == 'function')
end

-- TODO: Test: hookcall meta
do
	assert(type(multilua.hookcall) == 'function')
end

-- TODO: Test: hookreturn
do
	assert(type(multilua.hookreturn) == 'function')
end

-- TODO: Test: hookreturn meta
do
	assert(type(multilua.hookreturn) == 'function')
end

-- TODO: Test: hookline
do
	assert(type(multilua.hookline) == 'function')
end

-- TODO: Test: hookline meta
do
	assert(type(multilua.hookline) == 'function')
end

-- TODO: Test: hookcount
do
	assert(type(multilua.hookcount) == 'function')
end

-- TODO: Test: hookcount meta
do
	assert(type(multilua.hookcount) == 'function')
end

-- TODO: Test: hooktailcall
do
	assert(type(multilua.hooktailcall) == 'function')
end

-- TODO: Test: hooktailcall meta
do
	assert(type(multilua.hooktailcall) == 'function')
end

-- TODO: Test: maskcall
do
	assert(type(multilua.maskcall) == 'function')
end

-- TODO: Test: maskcall meta
do
	assert(type(multilua.maskcall) == 'function')
end

-- TODO: Test: maskreturn
do
	assert(type(multilua.maskreturn) == 'function')
end

-- TODO: Test: maskreturn meta
do
	assert(type(multilua.maskreturn) == 'function')
end

-- TODO: Test: maskline
do
	assert(type(multilua.maskline) == 'function')
end

-- TODO: Test: maskline meta
do
	assert(type(multilua.maskline) == 'function')
end

-- TODO: Test: maskcount
do
	assert(type(multilua.maskcount) == 'function')
end

-- TODO: Test: maskcount meta
do
	assert(type(multilua.maskcount) == 'function')
end

-- TODO: Test: pushunsigned
do
	assert(type(multilua.pushunsigned) == 'function')
end

-- TODO: Test: pushunsigned meta
do
	assert(type(multilua.pushunsigned) == 'function')
end

-- TODO: Test: pushuinteger
do
	assert(type(multilua.pushuinteger) == 'function')
end

-- TODO: Test: pushuinteger meta
do
	assert(type(multilua.pushuinteger) == 'function')
end

-- TODO: Test: tounsignedx
do
	assert(type(multilua.tounsignedx) == 'function')
end

-- TODO: Test: tounsignedx meta
do
	assert(type(multilua.tounsignedx) == 'function')
end

-- TODO: Test: tounsigned
do
	assert(type(multilua.tounsigned) == 'function')
end

-- TODO: Test: tounsigned meta
do
	assert(type(multilua.tounsigned) == 'function')
end

-- TODO: Test: getlocal
do
	assert(type(multilua.getlocal) == 'function')
end

-- TODO: Test: getlocal meta
do
	assert(type(multilua.getlocal) == 'function')
end

-- TODO: Test: setlocal
do
	assert(type(multilua.setlocal) == 'function')
end

-- TODO: Test: setlocal meta
do
	assert(type(multilua.setlocal) == 'function')
end
