SHELL = /bin/sh
CC    = gcc
FLAGS        = -std=gnu99 -Iinclude
TEST_FLAGS   = -Wall -Werror
TEST_CFLAGS  = -fPIC -g

SRC  = src
TEST = test
TEST_DEPS = test_deps

TEST_DIR           = test
TEST_RUNNER_SOURCE = $(TEST_DIR)/runner.c
TEST_RUNNER        = $(TEST_DIR)/runner

TEST_SOURCES = $(shell echo test/*_test.c)
TEST_HEADERS = $(shell echo test/*.h)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
TEST_DYLIBS  = $(TEST_OBJECTS:.o=.so)

$(TEST): $(TEST_DEPS)
	$(TEST_RUNNER) $(TEST_DIR)

$(TEST_DEPS):
	$(CC) -c $(TEST_FLAGS) $(TEST_SOURCES) -I $(SRC) -o $(TEST_OBJECTS)
	$(CC) -shared -o $(TEST_DYLIBS) $(TEST_OBJECTS)
	$(CC) $(TEST_RUNNER_SOURCE) -o $(TEST_RUNNER)
