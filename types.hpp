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
typedef short BoardIndex;
enum PIECE_TYPE {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NO_PIECE};
