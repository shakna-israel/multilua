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

-- TODO: Test self-library manipulation...
do
	--local obj = multilua.current()
	-- TODO: Lotta tests.
	--assert(obj:close() == nil)
end