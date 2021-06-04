local multilua = require "multilua"

for i = 1, 300000 do
	local obj = multilua.new()
	collectgarbage('collect')
end
