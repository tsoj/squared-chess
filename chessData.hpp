#pragma once

#include <cstdint>
#include <string>

#include "countZeros.hpp"

namespace chessData
{
  const uint64_t BIT_AT_INDEX[64] =
  #include "chessData/bitAtIndex.in"
  ;
  const uint64_t RANKS[8] =
  #include "chessData/ranks.in"
  ;
  const uint64_t FILES[8] =
  #include "chessData/files.in"
  ;
  const uint64_t RANKS_64[64] =
  #include "chessData/ranks64.in"
  ;
  const uint64_t FILES_64[64] =
  #include "chessData/files64.in"
  ;
  const uint64_t DIAGONALS_64[64] =
  #include "chessData/diagonals64.in"
  ;
  const uint64_t ANTI_DIAGONALS_64[64] =
  #include "chessData/antiDiagonals64.in"
  ;
  const uint64_t MAIN_DIAGONAL = 0b1000000001000000001000000001000000001000000001000000001000000001; //A1 to H8
  const uint64_t ANTI_DIAGONAL = 0b0000000100000010000001000000100000010000001000000100000010000000; //H1 to A8

  const size_t CASTLING_QUEEN_SIDE_ROOK_FROM_INDEX[2] = {0, 56};
  const uint64_t CASTLING_QUEEN_SIDE_ROOK_FROM[2] = {BIT_AT_INDEX[CASTLING_QUEEN_SIDE_ROOK_FROM_INDEX[0]], BIT_AT_INDEX[CASTLING_QUEEN_SIDE_ROOK_FROM_INDEX[1]]};
  const size_t CASTLING_KING_SIDE_ROOK_FROM_INDEX[2] = {7, 63};
  const uint64_t CASTLING_KING_SIDE_ROOK_FROM[2] = {BIT_AT_INDEX[CASTLING_KING_SIDE_ROOK_FROM_INDEX[0]], BIT_AT_INDEX[CASTLING_KING_SIDE_ROOK_FROM_INDEX[1]]};
  const size_t CASTLING_QUEEN_SIDE_ROOK_TO_INDEX[2] = {3, 59};
  const uint64_t CASTLING_QUEEN_SIDE_ROOK_TO[2] = {BIT_AT_INDEX[CASTLING_QUEEN_SIDE_ROOK_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_QUEEN_SIDE_ROOK_TO_INDEX[1]]};
  const size_t CASTLING_KING_SIDE_ROOK_TO_INDEX[2] = {5, 61};
  const uint64_t CASTLING_KING_SIDE_ROOK_TO[2] = {BIT_AT_INDEX[CASTLING_KING_SIDE_ROOK_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_KING_SIDE_ROOK_TO_INDEX[1]]};
  const size_t CASTLING_KING_FROM_INDEX[2] = {4, 60};
  const uint64_t CASTLING_KING_FROM[2] = {BIT_AT_INDEX[CASTLING_KING_FROM_INDEX[0]], BIT_AT_INDEX[CASTLING_KING_FROM_INDEX[1]]};
  const size_t CASTLING_QUEEN_SIDE_KING_TO_INDEX[2] = {2, 58};
  const uint64_t CASTLING_QUEEN_SIDE_KING_TO[2] = {BIT_AT_INDEX[CASTLING_QUEEN_SIDE_KING_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_QUEEN_SIDE_KING_TO_INDEX[1]]};
  const size_t CASTLING_KING_SIDE_KING_TO_INDEX[2] = {6, 62};
  const uint64_t CASTLING_KING_SIDE_KING_TO[2] = {BIT_AT_INDEX[CASTLING_KING_SIDE_KING_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_KING_SIDE_KING_TO_INDEX[1]]};
  const size_t CASTLING_QUEEN_SIDE_CHECK_RELEVANT_FIELDS[2][2] = {{3,4}, {59,60}};
  const size_t CASTLING_KING_SIDE_CHECK_RELEVANT_FIELDS[2][2] = {{4,5}, {60,61}};
  const uint64_t CASTLING_QUEEN_SIDE_BLOCK_RELEVANT_AREA[2] =
      { BIT_AT_INDEX[1] | BIT_AT_INDEX[2] | BIT_AT_INDEX[3], BIT_AT_INDEX[57] | BIT_AT_INDEX[58] | BIT_AT_INDEX[59] };
  const uint64_t CASTLING_KING_SIDE_BLOCK_RELEVANT_AREA[2] = { BIT_AT_INDEX[5] | BIT_AT_INDEX[6], BIT_AT_INDEX[61] | BIT_AT_INDEX[62] };

  const uint64_t RANK_ATTACK_TABLE[64][64] =
  #include "chessData/rankAttackTable.in"
  ;
  const uint64_t FILE_ATTACK_TABLE[64][64] =
  #include "chessData/fileAttackTable.in"
  ;
  const uint64_t DIAGONAL_ATTACK_TABLE[64][64] =
  #include "chessData/diagonalAttackTable.in"
  ;
  const uint64_t ANTI_DIAGONAL_ATTACK_TABLE[64][64] =
  #include "chessData/antiDiagonalAttackTable.in"
  ;
  const uint64_t KNIGHT_ATTACK_TABLE[64] =
  #include "chessData/knightAttackTable.in"
  ;
  const uint64_t KING_ATTACK_TABLE[64] =
  #include "chessData/kingAttackTable.in"
  ;
  const uint64_t PAWN_QUIET_ATTACK_TABLE[2][64] =
  #include "chessData/pawnQuietAttackTable.in"
  ;
  const uint64_t PAWN_CAPTURE_ATTACK_TABLE[2][64] =
  #include "chessData/pawnCaptureAttackTable.in"
  ;
  const uint64_t IS_PASSED[2][64] =
  #include "chessData/isPassed.in"
  ;
  const uint64_t HOME_RANK[2] = {RANKS[0], RANKS[7]};
  const uint64_t PAWN_HOME_RANK[2] = {RANKS[1], RANKS[6]};
  const uint64_t CENTER_7X7 = ~0 & ~RANKS[0] & ~RANKS[7] & ~FILES[0] & ~FILES[7];
  const int8_t MOVE_DIRECTION[2] = {1, -1};

  const uint64_t ZOBRIST_RANDOM_BITMASKS_PIECES[6][64] =
  #include "chessData/zobristRandomBitmasksPieces.in"
  ;
  const uint64_t ZOBRISTRANDOM_BITMASKS_PLAYERS[2][64] =
  #include "chessData/zobristRandomBitmasksPlayers.in"
  ;


  size_t getHashkeyRank(size_t index, uint64_t occupancy)
  {
    return ((occupancy >> ((index / 8)*8)) >> 1) & 0b111111U;
  }
  size_t getHashkeyFile(size_t index, uint64_t occupancy)
  {
    return (((((occupancy >> (index % 8)) & FILES[0] )*MAIN_DIAGONAL) >> 56) >> 1) & 0b111111U;
  }
  size_t getHashkeyDiagonal(size_t index, uint64_t occupancy)
  {
    return ((((occupancy & DIAGONALS_64[index])*FILES[0]) >> 56) >> 1) & 0b111111U;
  }
  size_t getHashkeyAntiDiagonal(size_t index, uint64_t occupancy)
  {
    return ((((occupancy & ANTI_DIAGONALS_64[index])*FILES[0]) >> 56) >> 1) & 0b111111U;
  }

  uint64_t getAttackMaskKnight(size_t index, uint64_t occupancy)
  {
    return KNIGHT_ATTACK_TABLE[index];
  }
  uint64_t getAttackMaskBishop(size_t index, uint64_t occupancy)
  {
    return ANTI_DIAGONAL_ATTACK_TABLE[index][getHashkeyAntiDiagonal(index, occupancy)] |
    DIAGONAL_ATTACK_TABLE[index][getHashkeyDiagonal(index, occupancy)];
  }
  uint64_t getAttackMaskRook(size_t index, uint64_t occupancy)
  {
    return RANK_ATTACK_TABLE[index][getHashkeyRank(index, occupancy)] |
    FILE_ATTACK_TABLE[index][getHashkeyFile(index, occupancy)];
  }
  uint64_t getAttackMaskQueen(size_t index, uint64_t occupancy)
  {
    return ANTI_DIAGONAL_ATTACK_TABLE[index][getHashkeyAntiDiagonal(index, occupancy)] |
    DIAGONAL_ATTACK_TABLE[index][getHashkeyDiagonal(index, occupancy)] |
    RANK_ATTACK_TABLE[index][getHashkeyRank(index, occupancy)] |
    FILE_ATTACK_TABLE[index][getHashkeyFile(index, occupancy)];
  }
  uint64_t getAttackMaskKing(size_t index, uint64_t occupancy)
  {
    return KING_ATTACK_TABLE[index];
  }

  size_t findAndClearTrailingOne(uint64_t & mask)
  {
    size_t ret = CountZeros::trailingZeros(mask);
    mask &= mask -1;
    return ret;
  }

  std::string getField_notation(size_t i)
  {
    switch(i)
    {
      case  0: return "a1";
      case  1: return "b1";
      case  2: return "c1";
      case  3: return "d1";
      case  4: return "e1";
      case  5: return "f1";
      case  6: return "g1";
      case  7: return "h1";
      case  8: return "a2";
      case  9: return "b2";
      case 10: return "c2";
      case 11: return "d2";
      case 12: return "e2";
      case 13: return "f2";
      case 14: return "g2";
      case 15: return "h2";
      case 16: return "a3";
      case 17: return "b3";
      case 18: return "c3";
      case 19: return "d3";
      case 20: return "e3";
      case 21: return "f3";
      case 22: return "g3";
      case 23: return "h3";
      case 24: return "a4";
      case 25: return "b4";
      case 26: return "c4";
      case 27: return "d4";
      case 28: return "e4";
      case 29: return "f4";
      case 30: return "g4";
      case 31: return "h4";
      case 32: return "a5";
      case 33: return "b5";
      case 34: return "c5";
      case 35: return "d5";
      case 36: return "e5";
      case 37: return "f5";
      case 38: return "g5";
      case 39: return "h5";
      case 40: return "a6";
      case 41: return "b6";
      case 42: return "c6";
      case 43: return "d6";
      case 44: return "e6";
      case 45: return "f6";
      case 46: return "g6";
      case 47: return "h6";
      case 48: return "a7";
      case 49: return "b7";
      case 50: return "c7";
      case 51: return "d7";
      case 52: return "e7";
      case 53: return "f7";
      case 54: return "g7";
      case 55: return "h7";
      case 56: return "a8";
      case 57: return "b8";
      case 58: return "c8";
      case 59: return "d8";
      case 60: return "e8";
      case 61: return "f8";
      case 62: return "g8";
      case 63: return "h8";
      default: return "??";
    }
  }

  size_t getField_index(std::string s)
  {
    if(s == "a1" | s == "A1"){ return  0; }
    if(s == "b1" | s == "B1"){ return  1; }
    if(s == "c1" | s == "C1"){ return  2; }
    if(s == "d1" | s == "D1"){ return  3; }
    if(s == "e1" | s == "E1"){ return  4; }
    if(s == "f1" | s == "F1"){ return  5; }
    if(s == "g1" | s == "G1"){ return  6; }
    if(s == "h1" | s == "H1"){ return  7; }
    if(s == "a2" | s == "A2"){ return  8; }
    if(s == "b2" | s == "B2"){ return  9; }
    if(s == "c2" | s == "C2"){ return 10; }
    if(s == "d2" | s == "D2"){ return 11; }
    if(s == "e2" | s == "E2"){ return 12; }
    if(s == "f2" | s == "F2"){ return 13; }
    if(s == "g2" | s == "G2"){ return 14; }
    if(s == "h2" | s == "H2"){ return 15; }
    if(s == "a3" | s == "A3"){ return 16; }
    if(s == "b3" | s == "B3"){ return 17; }
    if(s == "c3" | s == "C3"){ return 18; }
    if(s == "d3" | s == "D3"){ return 19; }
    if(s == "e3" | s == "E3"){ return 20; }
    if(s == "f3" | s == "F3"){ return 21; }
    if(s == "g3" | s == "G3"){ return 22; }
    if(s == "h3" | s == "H3"){ return 23; }
    if(s == "a4" | s == "A4"){ return 24; }
    if(s == "b4" | s == "B4"){ return 25; }
    if(s == "c4" | s == "C4"){ return 26; }
    if(s == "d4" | s == "D4"){ return 27; }
    if(s == "e4" | s == "E4"){ return 28; }
    if(s == "f4" | s == "F4"){ return 29; }
    if(s == "g4" | s == "G4"){ return 30; }
    if(s == "h4" | s == "H4"){ return 31; }
    if(s == "a5" | s == "A5"){ return 32; }
    if(s == "b5" | s == "B5"){ return 33; }
    if(s == "c5" | s == "C5"){ return 34; }
    if(s == "d5" | s == "D5"){ return 35; }
    if(s == "e5" | s == "E5"){ return 36; }
    if(s == "f5" | s == "F5"){ return 37; }
    if(s == "g5" | s == "G5"){ return 38; }
    if(s == "h5" | s == "H5"){ return 39; }
    if(s == "a6" | s == "A6"){ return 40; }
    if(s == "b6" | s == "B6"){ return 41; }
    if(s == "c6" | s == "C6"){ return 42; }
    if(s == "d6" | s == "D6"){ return 43; }
    if(s == "e6" | s == "E6"){ return 44; }
    if(s == "f6" | s == "F6"){ return 45; }
    if(s == "g6" | s == "G6"){ return 46; }
    if(s == "h6" | s == "H6"){ return 47; }
    if(s == "a7" | s == "A7"){ return 48; }
    if(s == "b7" | s == "B7"){ return 49; }
    if(s == "c7" | s == "C7"){ return 50; }
    if(s == "d7" | s == "D7"){ return 51; }
    if(s == "e7" | s == "E7"){ return 52; }
    if(s == "f7" | s == "F7"){ return 53; }
    if(s == "g7" | s == "G7"){ return 54; }
    if(s == "h7" | s == "H7"){ return 55; }
    if(s == "a8" | s == "A8"){ return 56; }
    if(s == "b8" | s == "B8"){ return 57; }
    if(s == "c8" | s == "C8"){ return 58; }
    if(s == "d8" | s == "D8"){ return 59; }
    if(s == "e8" | s == "E8"){ return 60; }
    if(s == "f8" | s == "F8"){ return 61; }
    if(s == "g8" | s == "G8"){ return 62; }
    if(s == "h8" | s == "H8"){ return 63; }
    return 64;
  }
}
