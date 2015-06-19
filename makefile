CC = g++
CFLAGS = -Wall
INCLUDES = -I/usr/include/python2.7
LIBS = -lpython2.7
EXECUTABLES = $(patsubst %.cpp, %, $(wildcard *.cpp))

.PHONY: clean

all: $(EXECUTABLES)
	@echo Compile tutorials for embedding python

%: %.cpp
	@$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $< -o $@ -lm

clean:
	@$(RM) $(EXECUTABLES)
