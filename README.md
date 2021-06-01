# multilua

A Lua 5.3 library for accessing the C-API.

---

## About

This is a Lua 5.3 library for accessing and managing individual Lua states as if by the C-API, but within Lua itself.

e.g.

	local multilua = require "multilua"

	local L = assert(multilua.new())
	L:openlibs()
	
	L[#L + 1] = "Hello,"
	L[#L + 1] = " "
	L[#L + 1] = "World!"
	assert(L:concat(3))
	assert(L(-1) == "Hello, World!")

	L:close()

Note: You can have as many Lua states as you want. The memory overhead is surprisingly small, and states don't interact with each other.

Note: You don't _need_ to call `close`. The GC will also close it for you, if the object goes out of scope.

---

## Status

Alpha. Unversioned.

Bugs should be reported to the official [Issue Tracker](https://todo.sr.ht/~shakna/multilua).

It "works", but is undocumented, untested, and may be... Unpredictable.

It doesn't quite obey all the conventions of the C-API, because it isn't sensible to in some places.

Which makes this probably _unsafe_ unless you know exactly what you are doing.

---

## Documentation

The documentation is currently under significant development and _incredibly incomplete_.

You can find a rendered form at `book.txt`.

You can also generate pdf, epub, and html forms using [`bookwriter`](https://git.sr.ht/~shakna/bookwriter).

---

## Building

The included `Makefile` can generate the shared library for you:

	make

Place the resulting `multilua.so` somewhere in `LUA_CPATH`. (Which, by default, can be the root directory of your project.)

### Dependencies

+ Lua 5.3 & headers.

+ pkg-config

+ make

+ C99 compatible C compiler.

### Documentation Dependencies (Optional)

Generating the documentation also requires [`bookwriter`](https://git.sr.ht/~shakna/bookwriter).

	make doc

### Testing

Testing has no additional dependencies.

	make test

---

## License

See the `LICENSE` file.
