#Compiler
CC          := g++

SRCDIR      := src
TARGETDIR   := bin
SRCEXT      := cpp

SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Flags
CFLAGS      := -Wall -Werror -ansi -pedantic

all: bin/rshell

bin/rshell: bin/rshell.o
	$(CC) $(CFLAGS) -L$(SRCDIR)/ -o $@ $^

$(TARGETDIR)/rshell.o: src/rshell.cpp bin/
	$(CC) -c src/rshell.cpp $(CFLAGS) -o $(TARGETDIR)/rshell.o
	
bin/:
	mkdir bin
	
clean:
	rm -f bin/*.o bin/rshell