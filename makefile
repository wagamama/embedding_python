CC = gcc
EXECUTABLES = $(patsubst %.c, %, $(wildcard *.c))

.PHONY: clean

all: $(EXECUTABLES)
	@echo Compile tutorials for embedding python

%: %.c
	@$(CC) `python-config --cflags` $< -o $@ `python-config --ldflags`

clean:
	@$(RM) $(EXECUTABLES)
