local multilua = require "multilua"

-- Test: We got a library
assert(multilua ~= nil)
assert(type(multilua) == 'table')

-- Test: We have a library version
assert(multilua.version ~= nil)

print("---")
for k, v in pairs(multilua) do
	print(k, v)
end
print("---")

-- Test: creation
do
	assert(type(multilua.new) == 'function')
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	-- TODO: remove this print
	for k, v in pairs(obj) do
		print(k, v)
	end

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


-- TODO: Test: openlibs
do
	assert(type(multilua.openlibs) == 'function')
end

-- TODO: Test: openlibs meta
do
	assert(type(multilua.openlibs) == 'function')
end

-- TODO: Test: absindex
do
	assert(type(multilua.absindex) == 'function')
end

-- TODO: Test: absindex meta
do
	assert(type(multilua.absindex) == 'function')
end

-- TODO: Test: arith
do
	assert(type(multilua.arith) == 'function')
end

-- TODO: Test: arith meta
do
	assert(type(multilua.arith) == 'function')
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

-- TODO: Test: compare
do
	assert(type(multilua.compare) == 'function')
end

-- TODO: Test: compare meta
do
	assert(type(multilua.compare) == 'function')
end

-- TODO: Test: concat
do
	assert(type(multilua.concat) == 'function')
end

-- TODO: Test: concat meta
do
	assert(type(multilua.concat) == 'function')
end

-- TODO: Test: copy
do
	assert(type(multilua.copy) == 'function')
end

-- TODO: Test: copy meta
do
	assert(type(multilua.copy) == 'function')
end

-- TODO: Test: createtable
do
	assert(type(multilua.createtable) == 'function')
end

-- TODO: Test: createtable meta
do
	assert(type(multilua.createtable) == 'function')
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

-- TODO: Test: gettop
do
	assert(type(multilua.gettop) == 'function')
end

-- TODO: Test: gettop meta
do
	assert(type(multilua.gettop) == 'function')
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

-- TODO: Test: iscfunction
do
	assert(type(multilua.iscfunction) == 'function')
end

-- TODO: Test: iscfunction meta
do
	assert(type(multilua.iscfunction) == 'function')
end

-- TODO: Test: isfunction
do
	assert(type(multilua.isfunction) == 'function')
end

-- TODO: Test: isfunction meta
do
	assert(type(multilua.isfunction) == 'function')
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

-- TODO: Test self-library manipulation...
do
	-- Test: Creation
	local obj = multilua.current()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	-- Closing the current state is ignored:
	-- (This will segfault if it fails...)
	assert(multilua.close(obj) == nil)

	-- Closing the current state is ignored:
	obj = multilua.current()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')
	assert(obj:close() == nil)

	-- TODO: Lotta tests.
end
