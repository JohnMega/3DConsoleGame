# Linux/macOS build for 3DConsoleGame.
# Windows users: keep using the Visual Studio solution (3dFullGameClean.sln).
#
# Usage:
#   make            # build ./3dgame
#   make run        # build then run (must be a real terminal >= 63x26)
#   make clean

CXX      ?= g++
CXXFLAGS ?= -std=c++20 -O2 -w -MMD -MP
LDFLAGS  ?= -pthread

INCDIRS := source source/Actors source/bitmap source/Cameras source/Delegate \
           source/GlobalVars source/Light source/MapLoader source/Maths \
           source/Menu source/ModelsManager source/Multiplayer source/ObjFileParse \
           source/oof source/Pixel source/Render source/StaticMeshes source/Platform
INCFLAGS := $(addprefix -I,$(INCDIRS))

SRCS := 3dFullGameClean.cpp $(shell find source -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

BIN := 3dgame

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJS) $(DEPS) $(BIN)

-include $(DEPS)

.PHONY: all run clean
