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

Caveat: Some parts of the exposed API are missing, as Lua would struggle to make it easy to represent some things.

---

## Status

Unuseable. Literally.

Not only is this unuseable in its current state, it is completely undocumented and doesn't quite obey all the conventions of the C-API, because it isn't sensible to in some places.

Which makes this unuseable _and_ dangerous.

---

## License

See the `LICENSE` file.
