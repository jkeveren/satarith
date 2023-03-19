CC = g++
CFLAGS = -Wall -Wextra -Werror -pedantic -Iinclude

ifneq ($(ALL_TYPES), false)
CFLAGS += -DALL_TYPES
endif

build/test: test/test.cpp $(wildcard test/*) makefile build
	$(CC) $(CFLAGS) -o $@ $< \
	# Comiling templates...
	chmod u+x build/test

build/example: example/example.cpp makefile build
	$(CC) $(CFLAGS) -o $@ $<

build:
	mkdir -p $@

.PHONY: test
test: build/test
	./build/test
