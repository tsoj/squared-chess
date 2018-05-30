#pragma once

#include <cstdint>

#if defined(__GNUC__) || defined(__clang__)
  #ifdef __i386
    #define ARCH_X86
  #endif
  #ifdef __amd64
    #define ARCH_X86_64
  #endif
#endif

typedef uint64_t Bitboard;
enum PIECE_TYPE {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NO_PIECE};
enum COLOR_TYPE {WHITE, BLACK, NO_COLOR};
#define WHITE_TO_MOVE 1
#define BLACK_TO_MOVE -1

#define NORTH 8
#define SOUTH -8
#define EAST 1
#define WEST -1
#define NORTH_EAST 9
#define NORTH_WEST 7
#define SOUTH_WEST -9
#define SOUTH_EAST -7

typedef int Score;
#define SCORE_INFINITY 30000
#define SCORE_MATE 20000
