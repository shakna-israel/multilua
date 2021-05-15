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
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	-- TODO: remove this print
	for k, v in pairs(obj) do
		print(k, v)
	end

	-- Test: closing
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


-- TODO: Test: openlibs
-- TODO: Test: openlibs meta

-- TODO: Test: absindex
-- TODO: Test: absindex meta

-- TODO: Test: arith
-- TODO: Test: arith meta

-- TODO: Test: call
-- TODO: Test: call meta

-- Test: checkstack
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(multilua.checkstack(obj, 2) == true)

	obj:close()
end

-- Test: checkstack meta
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(obj:checkstack(2) == true)

	obj:close()
end

-- TODO: Test: compare
-- TODO: Test: compare meta

-- TODO: Test: concat
-- TODO: Test: concat meta

-- TODO: Test: copy
-- TODO: Test: copy meta

-- TODO: Test: createtable
-- TODO: Test: createtable meta

-- Can we even test `error`??
-- TODO: Test: error
-- TODO: Test: error meta

-- Can we even test `gc`??
-- TODO: Test: gc
-- TODO: Test: gc meta

-- TODO: Test: getfield
-- TODO: Test: getfield meta

-- Test: luaversion
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(multilua.luaversion(obj) == 503.0)

	obj:close()
end

-- Test: luaversion meta
do
	local obj = multilua.new()
	assert(type(obj) == 'table')
	assert(type(obj.self) == 'userdata')

	assert(obj:luaversion() == 503.0)

	obj:close()
end

-- TODO: Test: xmove
-- TODO: Test: xmove meta

-- TODO: Test: yield
-- TODO: Test: yield meta

-- TODO: Test: getglobal
-- TODO: Test: getglobal meta

-- TODO: Test: geti
-- TODO: Test: geti meta

-- TODO: Test: getmetatable
-- TODO: Test: getmetatable meta

-- TODO: Test: gettable
-- TODO: Test: gettable meta

-- TODO: Test: gettop
-- TODO: Test: gettop meta

-- TODO: Test: getuservalue
-- TODO: Test: getuservalue meta

-- TODO: Test: insert
-- TODO: Test: insert meta

-- TODO: Test: isboolean
-- TODO: Test: isboolean meta

-- TODO: Test: iscfunction
-- TODO: Test: iscfunction meta

-- TODO: Test: isfunction
-- TODO: Test: isfunction meta

-- TODO: Test: isinteger
-- TODO: Test: isinteger meta

-- TODO: Test self-library manipulation...
do
	--local obj = multilua.current()
	-- TODO: Lotta tests.
	--assert(obj:close() == nil)
end
