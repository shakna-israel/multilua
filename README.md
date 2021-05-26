# multilua

A Lua library for accessing the C-API.

---

## About

This is a Lua library for accessing and managing individual Lua states as if by the C-API, but within Lua itself.

e.g.

	local multilua = require "multilua"

	local L = assert(multilua.new())
	L:openlibs()
	
	...

	L:close()

---

## Status

Alpha.

It "works", but is undocumented, untested, and may be... Unpredictable.

This is completely undocumented and doesn't quite obey all the conventions of the C-API, because it isn't sensible to in some places.

Which makes this probably _unsafe_ unless you know exactly what you are doing.

---

## License

See the `LICENSE` file.
