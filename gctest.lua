local multilua = require "multilua"

for i = multilua.mininteger(), multilua.maxinteger() do
	local obj = multilua.new()
	i = i + 1
	print(i)
end
