#pragma once

#include "types.hpp"
#include <array>

#define MAIN_DIAGONAL 0b1000000001000000001000000001000000001000000001000000001000000001 //A1 to H8
#define ANTI_DIAGONAL 0b0000000100000010000001000000100000010000001000000100000010000000 //H1 to A8

namespace BitMasks
{
  Bitboard rankToFile(Bitboard rank);

  int getHashKeyForRank(int fieldIndex, Bitboard occupancy);
  int getHashKeyForFile(int fieldIndex, Bitboard occupancy);
  int getHashKeyForDiagonal(int fieldIndex, Bitboard occupancy);
  int getHashKeyForAntiDiagonal(int fieldIndex, Bitboard occupancy);


  extern const std::array<Bitboard, 64> bitAtIndex;
  extern const std::array<Bitboard, 8> ranks;
  extern const std::array<Bitboard, 8> files;
  extern const std::array<Bitboard, 64> ranks64;
  extern const std::array<Bitboard, 64> files64;
  extern const std::array<Bitboard, 64> diagonals64;
  extern const std::array<Bitboard, 64> antiDiagonals64;
  extern const std::array<Bitboard, 64> possibleRanks;
  extern const std::array<Bitboard, 64> possibleFiles;

  extern const std::array<std::array<Bitboard, 64>, 64> antiDiagonalAttackTable;
  extern const std::array<std::array<Bitboard, 64>, 64> diagonalAttackTable;
  extern const std::array<std::array<Bitboard, 64>, 64> fileAttackTable;
  extern const std::array<std::array<Bitboard, 64>, 64> intAttackTable;
  extern const std::array<Bitboard, 64> knightAttackTable;
  extern const std::array<Bitboard, 64> kingAttackTable;
  extern const std::array<Bitboard, 64> pawnWhiteAttackTable;
  extern const std::array<Bitboard, 64> pawnBlackAttackTable;

  /*template<PIECE_TYPE Pt>
  int getAttackBitboard(Bitboard occupancy, BoardIndex boardIndex)
  {
      return Bitboard();
  }

  template<>
  int getAttackBitboard<NO_PIECE>(Bitboard occupancy, BoardIndex boardIndex)
  {
      return 0;
  }

  template<>
  int getAttackBitboard<BISHOP>(Bitboard occupancy, BoardIndex boardIndex)
  {
      return 0;
  }*/
};
