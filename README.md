# multilua

A Lua 5.3 library for accessing Lua's C-API.

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

You can have as many Lua states as you want. The memory overhead is surprisingly small.

Note: You don't _need_ to call `close`. The GC will also close it for you, if the object goes out of scope.

---

## Status

[![builds.sr.ht status](https://builds.sr.ht/~shakna/multilua.svg)](https://builds.sr.ht/~shakna/multilua?)

Alpha. Unversioned.

Bugs should be reported to the official [Issue Tracker](https://todo.sr.ht/~shakna/multilua).

It "works", but is only partially documented, and not fully tested.

It doesn't quite obey all the conventions of the C-API, because it isn't sensible to in some places. (Such as using strings instead of enums).

---

## Documentation

The documentation is currently under significant development and _incomplete_.

You can find a rendered form at [`book.txt`](https://git.sr.ht/~shakna/multilua/blob/master/book.txt).

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

On Debian/Ubuntu/etc:

	apt-get install build-essential pkg-config lua5.3-dev

On Alpine:

	apk add lua5.3 lua5.3-dev make

On Arch:

	pacman -S lua53 pkgconf make gcc

### Testing

Testing has no additional dependencies.

	make test

### Documentation Dependencies (Optional)

Generating the documentation also requires [`bookwriter`](https://git.sr.ht/~shakna/bookwriter).

	make doc

---

## License

See the `LICENSE` file.
