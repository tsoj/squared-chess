CC      = clang++
CFLAGS  = -std=c++17 -O3
LDFLAGS = -std=c++17 -O3
NAME = ^2-chess
BIN_FILE_PATH = ./bin/

#HPP = bitMasks.hpp
CPP = main.cpp bitMasks.cpp utils.cpp position.cpp search.cpp
OBJ = $(CPP:%.cpp=%.o)
OBJ_DEST = $(CPP:%.cpp=$(BIN_FILE_PATH)%.o)

all: $(OBJ)
	$(CC) -o $(BIN_FILE_PATH)$(NAME) $(OBJ_DEST) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $(BIN_FILE_PATH)$@

clean:
	rm $(OBJ_DEST) $(BIN_FILE_PATH)$(NAME)
