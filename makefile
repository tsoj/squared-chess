CC      = clang++
CFLAGS  = -std=c++14 -O3
LDFLAGS = -std=c++14 -O3
NAME = ^2-chess
BIN_FILE_PATH = ./bin/

CPP = main.cpp
OBJ = $(CPP:%.cpp=%.o)
OBJ_DEST = $(CPP:%.cpp=$(BIN_FILE_PATH)%.o)

build:
	$(CC) $(CFLAGS) $(LDFLAGS) -o build build.cpp && ./build && rm ./build

all: build $(OBJ)
	$(CC) -o $(BIN_FILE_PATH)$(NAME) $(OBJ_DEST) $(LDFLAGS)

test: all
	$(BIN_FILE_PATH)$(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $(BIN_FILE_PATH)$@

clean:
	rm $(OBJ_DEST) $(BIN_FILE_PATH)$(NAME)
