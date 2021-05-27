# multilua

A Lua 5.3 library for accessing the C-API.

---

## About

This is a Lua 5.3 library for accessing and managing individual Lua states as if by the C-API, but within Lua itself.

e.g.

	local multilua = require "multilua"

	local L = assert(multilua.new())
	L:openlibs()
	
	L:pushstring("Hello,")
	L:pushstring(" ")
	L:pushstring("World!")
	assert(L:concat(3))
	assert(L:tostring(-1) == "Hello, World!")

	L:close()

Note: You can have as many Lua states as you want. The memory overhead is surprisingly small, and states don't interact with each other.

Note: You don't _need_ to call `close`. The GC will also close it for you, if the object goes out of scope.

---

## Status

Alpha.

It "works", but is undocumented, untested, and may be... Unpredictable.

This is completely undocumented and doesn't quite obey all the conventions of the C-API, because it isn't sensible to in some places.

Which makes this probably _unsafe_ unless you know exactly what you are doing.

---

## Documentation

The documentation is currently under significant development and _incredibly incomplete_.

However, you can find a rendered form at `book.txt`.

You can also generate pdf, epub, and html forms using [`bookwriter`](https://git.sr.ht/~shakna/bookwriter).

---

## License

See the `LICENSE` file.
