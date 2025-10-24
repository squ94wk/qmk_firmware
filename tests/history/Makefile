# Makefile for history pattern matching tests

CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra
CXXFLAGS = -std=c++11 -Wall -Wextra

# Targets
.PHONY: all test clean

all: test

test: standalone_test.cpp ../../users/squ94wk/history_pattern_match.c
	@echo "Building standalone history pattern matching test..."
	$(CC) $(CFLAGS) -c ../../users/squ94wk/history_pattern_match.c -o history_pattern_match.o
	$(CXX) $(CXXFLAGS) standalone_test.cpp history_pattern_match.o -o test_history
	@echo "Running tests..."
	@./test_history

clean:
	rm -f test_history history_pattern_match.o

help:
	@echo "History Pattern Matching Tests"
	@echo ""
	@echo "Targets:"
	@echo "  make test   - Build and run standalone tests"
	@echo "  make clean  - Remove test binaries"
	@echo "  make help   - Show this help message"

