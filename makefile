CC = g++
CFLAGS += -Wall -Wextra -Werror -pedantic -Iinclude

build/test: test/test.cpp $(wildcard test/*) build
	$(CC) $(CFLAGS) -o $@ $<
	chmod u+x build/test

build/example: example/example.cpp build
	$(CC) $(CFLAGS) -o $@ $<

build: 
	mkdir -p $@

.PHONY: test
test: build/test
	./build/test
