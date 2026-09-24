SOURCES  := $(wildcard examples/*.c)
BINARIES := $(patsubst examples/%.c,bin/%,$(SOURCES))

.PHONY: all clean

all: $(BINARIES)

ifeq ($(OS),Windows_NT)
    MKDIR = if not exist bin mkdir bin
    RM = del /Q
    EXE = .exe
else
    MKDIR = mkdir -p bin
    RM = rm -f
    EXE =
endif

bin/%$(EXE): examples/%.c
	$(MKDIR)
	gcc -o $@ $<

clean:
	$(RM) $(BINARIES)
