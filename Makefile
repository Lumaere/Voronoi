# Reference:
# https://www.gnu.org/software/make/manual/make.html
#
# This makefile is largely from:
# http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
 
CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := bin/voronoi
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++14 -DDEBUG
LIB := 
INC := -I include

DIR_GUARD=@mkdir -p $(@D)

$(TARGET): $(OBJECTS) 
	@echo " Linking..." 
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) 
	$(DIR_GUARD)
	@echo " $(CC) $(CFLAGS) $(INC) -c -MMD -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -MMD -o $@ $<

clean: 
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester: 
	$(CC) $(CFLAGS) test/tester.cpp src/kruskal.cpp src/Fortunes.cpp src/beach_line.cpp src/node.cpp src/tree.cpp src/math/math.cpp src/DCEL/DCEL.cpp src/math/line.cpp $(INC) $(LIB) -o bin/tester

# Spikes (miscallaneous items)
ticket: 
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean

# print variables for debugging
print-%  : ; @echo $* = $($*)

# create dependency files for object files
-include $(OBJECTS:%.o=%.d)

