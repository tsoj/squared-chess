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

  template<PIECE_TYPE Pt>
  Bitboard getAttackBitboard(Bitboard occupancy, int boardIndex);
  template<>
  Bitboard getAttackBitboard<PAWN>(Bitboard occupancy, int boardIndex);
  template<>
  Bitboard getAttackBitboard<KNIGHT>(Bitboard occupancy, int boardIndex);
  template<>
  Bitboard getAttackBitboard<BISHOP>(Bitboard occupancy, int boardIndex);
  template<>
  Bitboard getAttackBitboard<ROOK>(Bitboard occupancy, int boardIndex);
  template<>
  Bitboard getAttackBitboard<QUEEN>(Bitboard occupancy, int boardIndex);
  template<>
  Bitboard getAttackBitboard<KING>(Bitboard occupancy, int boardIndex);
};
