CC = g++
EXECUTABLES = $(patsubst %.cpp, %, $(wildcard *.cpp))

.PHONY: clean

all: $(EXECUTABLES)
	@echo Compile tutorials for embedding python

%: %.cpp
	@$(CC) `python-config --cflags` $< -o $@ `python-config --ldflags`

clean:
	@$(RM) $(EXECUTABLES)
