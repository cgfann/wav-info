# simple makefile for clang-tidy and compiling
#
# Nirre Pluf 
# 3/7/2021
# CSC 250 Lab 4

SOURCE = lab4.c
EXECUTABLE = lab4
COMPILE_FLAGS = -ansi -pedantic -Wall

all: 
	gcc $(COMPILE_FLAGS) -o $(EXECUTABLE) $(SOURCE)

tidy: $(SOURCE)
	clang-tidy -checks='*' $(SOURCE) -- -std=c99

clean:
	rm -rf $(EXECUTABLE)

