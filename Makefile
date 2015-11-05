# Compiler:
CC = g++-4.9

# Flags:
CFLAGS = -O3

# Framework flags:
FRAMEWORKS = -lsgct -framework Opengl -framework Cocoa -framework IOKit -framework CoreVideo -std=c++11

LIBFOLD = -L"/usr/local/lib"
INCFOLD = -I"/usr/local/include"


# Files:
FILES = $(wildcard src/*.cpp)

# Binary folder:
BINFOLD = bin/

# Binary name:
BINNAME = Rainbone

FOO = this is not windows



all: compile
.PHONY: all

compile: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(BINFOLD)$(BINNAME) $(LIBFOLD) $(INCFOLD) $(FRAMEWORKS)
.PHONY: compile

run:
	./$(BINFOLD)$(BINNAME) -config "config/single.xml"
.PHONY: run

run-fisheye:
	./$(BINFOLD)$(BINNAME) -config "config/single_fisheye_fxaa.xml"
.PHONY: run

clean:
	rm -f $(BINFOLD)*
.PHONY: clean
