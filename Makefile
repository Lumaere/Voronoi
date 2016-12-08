# Reference:
# https://www.gnu.org/software/make/manual/make.html
# 

CC = g++
LD = g++

CFLAGS = -pedantic-errors -std=c++14 -Wall -fno-elide-constructors
LFLAGS = -pedantic-errors -Wall
DFLAGS = -g -DDEBUG
RFLAGS = -O3

SOURCE_FILES = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)

OBJ = objs
SRC = src
EXECUTABLE_NAME = voronoi

# Hopefully standard implementation below

OBJECT_FILES = $(addprefix $(OBJ)/,$(SOURCE_FILES:%.cpp=%.o))
EXECUTABLE_FILES = $(EXECUTABLE_NAME)

build: $(EXECUTABLE_FILES)

$(EXECUTABLE_FILES): $(OBJECT_FILES)
		$(LD) $(DFLAGS) $(LFLAGS) $^ -o $@
		dsymutil $(EXECUTABLE_NAME)

$(OBJ)/src/%.o: $(SRC)/%.cpp
		$(CC) $(DFLAGS) $(CFLAGS) -c -MMD $< -o $@

.PHONY: clean
clean:
		rm -f $(OBJECT_FILES)
		rm -f $(OBJECT_FILES:%.o=%.d)
		rm -rf $(EXECUTABLE_FILES)*


-include $(OBJECT_FILES:%.o=%.d)

print-%  : ; @echo $* = $($*)
