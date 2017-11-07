#pragma once


#ifdef _MSC_VER
  #ifdef _M_IX86
    #define ARCH_X86
  #endif
  #ifdef _M_X64
    #define ARCH_X86_64
  #endif
#endif

#ifdef __INTEL_COMPILER
  #ifdef _M_IX86
    #define ARCH_X86
  #endif
  #ifdef __amd64
    #define ARCH_X86_64
  #endif
#endif

#if defined(__GNUC__) || defined(__clang__)
  #ifdef __i386
    #define ARCH_X86
  #endif
  #ifdef __amd64
    #define ARCH_X86_64
  #endif
#endif

#if defined( _WIN32) && defined(ARCH_X86)
  typedef unsigned long int uint32;
#else
  typedef unsigned int uint32;
#endif

typedef unsigned long long int uint64;
typedef uint64 Bitboard;
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

typedef int PositionValue;
#define POSITION_VALUE_INFINITY 30000
