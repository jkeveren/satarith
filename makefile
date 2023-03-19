CC = g++
CFLAGS += -Wall -Wextra -Werror -pedantic -Iinclude

build/test: test/test.cpp $(wildcard test/*) include/satarith.hpp build
	$(CC) $(CFLAGS) -o $@ $<
	chmod u+x build/test

build/example: example/example.cpp include/satarith.hpp build
	$(CC) $(CFLAGS) -o $@ $<

build: 
	mkdir $@

.PHONY: test
test: build/test
	./build/test
