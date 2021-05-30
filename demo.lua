local multilua = require "multilua"

do
	local obj = multilua.new()

	-- Put some stuff onto the stack
	for i=1, 10 do
		obj[#obj + 1] = i
	end
	obj:pushboolean(true)
	obj:pushstring("Hello, World!")
	obj:newtable()

	-- Print all types on stack:
	local i = #obj
	while i > 0 do
		local t = obj[i]
		
		if t == "nil" then
			print(string.format("%d) <nil>", i))
		elseif t == "number" then
			print(string.format("%d) <%s><%d>", i, t, obj:tonumber(i)))
		elseif t == "boolean" then
			if obj:toboolean(i) then
				print(string.format("%d) <%s><true>", i, t))
			else
				print(string.format("%d) <%s><false>", i, t))
			end
		elseif t == "string" then
			print(string.format("%d) <%s><%q>", i, t, obj:tostring(i)))
		else
			print(string.format("%d) <%s><%s>", i, t, obj:topointer(i)))
		end

		i = i - 1
	end

	-- Note: No need to cleanup obj, it'll be GC'd.
end
