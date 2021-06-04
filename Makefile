LUALIB=`pkg-config --libs --cflags lua5.3`
INCLUDE=-Iinclude

all: multilua.o
	$(CC) -g multilua.o -shared -o multilua.so

multilua.o:
	$(CC) -g $(LUALIB) $(INCLUDE) -c -fPIC -o multilua.o src/multilua.c

.PHONY: doc
doc:
	bookwriter render

.PHONY: test
test: clean all
	lua5.3 test.lua

.PHONY: clean
clean:
	-rm multilua.o
	-rm multilua.so

.PHONY: cleandoc
cleandoc:
	-rm book.pdf
	-rm book.html
	-rm book.epub
	-rm book.txt
