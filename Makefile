LUALIB=`pkg-config --libs --cflags lua5.3`
INCLUDE=-Iinclude

all: multilua.o
	$(CC) multilua.o -shared -o multilua.so

multilua.o:
	$(CC) $(LUALIB) $(INCLUDE) -c -fPIC -o multilua.o src/multilua.c

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
	-rm book.pdf
	-rm book.html
	-rm book.epub
	-rm book.txt
