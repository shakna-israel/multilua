local multilua = require "multilua"

do
	local obj = multilua.new()

	-- Put some stuff onto the stack
	for i=1, 10 do
		obj:pushinteger(i)
	end
	obj:pushboolean(true)
	obj:pushstring("Hello, World!")
	obj:newtable()

	print(#obj, obj:gettop())
	print(getmetatable(obj))

	-- Print all types on stack:
	local i = obj:gettop()
	while i > 0 do
		local t = obj:type(i)
		
		if t == "nil" then
			print("<nil>")
		elseif t == "number" then
			print(string.format("<%s><%d>", t, obj:tonumber(i)))
		elseif t == "boolean" then
			if obj:toboolean(i) then
				print(string.format("<%s><true>", t))
			else
				print(string.format("<%s><false>", t))
			end
		elseif t == "string" then
			print(string.format("<%s><%q>", t, obj:tostring(i)))
		else
			print(string.format("<%s><%s>", t, obj:topointer(i)))
		end

		i = i - 1
	end

	-- Note: No need to cleanup obj, it'll be GC'd.
end
