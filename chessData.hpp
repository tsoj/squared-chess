#pragma once

#include <cstdint>

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

  const size_t CASTLING_QUEENSIDE_ROOK_FROM_INDEX[2] = {0, 56};
  const uint64_t CASTLING_QUEENSIDE_ROOK_FROM[2] = {BIT_AT_INDEX[CASTLING_QUEENSIDE_ROOK_FROM_INDEX[0]], BIT_AT_INDEX[CASTLING_QUEENSIDE_ROOK_FROM_INDEX[1]]};
  const size_t CASTLING_KINGSIDE_ROOK_FROM_INDEX[2] = {7, 63};
  const uint64_t CASTLING_KINGSIDE_ROOK_FROM[2] = {BIT_AT_INDEX[CASTLING_KINGSIDE_ROOK_FROM_INDEX[0]], BIT_AT_INDEX[CASTLING_KINGSIDE_ROOK_FROM_INDEX[1]]};
  const size_t CASTLING_QUEENSIDE_ROOK_TO_INDEX[2] = {3, 59};
  const uint64_t CASTLING_QUEENSIDE_ROOK_TO[2] = {BIT_AT_INDEX[CASTLING_QUEENSIDE_ROOK_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_QUEENSIDE_ROOK_TO_INDEX[1]]};
  const size_t CASTLING_KINGSIDE_ROOK_TO_INDEX[2] = {5, 61};
  const uint64_t CASTLING_KINGSIDE_ROOK_TO[2] = {BIT_AT_INDEX[CASTLING_KINGSIDE_ROOK_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_KINGSIDE_ROOK_TO_INDEX[1]]};
  const size_t CASTLING_KING_FROM_INDEX[2] = {4, 60};
  const uint64_t CASTLING_KING_FROM[2] = {BIT_AT_INDEX[CASTLING_KING_FROM_INDEX[0]], BIT_AT_INDEX[CASTLING_KING_FROM_INDEX[1]]};
  const size_t CASTLING_QUEENSIDE_KING_TO_INDEX[2] = {2, 58};
  const uint64_t CASTLING_QUEENSIDE_KING_TO[2] = {BIT_AT_INDEX[CASTLING_QUEENSIDE_KING_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_QUEENSIDE_KING_TO_INDEX[1]]};
  const size_t CASTLING_KINGSIDE_KING_TO_INDEX[2] = {6, 62};
  const uint64_t CASTLING_KINGSIDE_KING_TO[2] = {BIT_AT_INDEX[CASTLING_KINGSIDE_KING_TO_INDEX[0]], BIT_AT_INDEX[CASTLING_KINGSIDE_KING_TO_INDEX[1]]};
  const size_t CASTLING_QUEENSIDE_CHECK_RELEVANT_FIELDS[2][2] = {{3,4}, {59,60}};
  const size_t CASTLING_KINGSIDE_CHECK_RELEVANT_FIELDS[2][2] = {{4,5}, {60,61}};
  const uint64_t CASTLING_QUEENSIDE_BLOCK_RELEVANT_AREA[2] =
      { BIT_AT_INDEX[1] | BIT_AT_INDEX[2] | BIT_AT_INDEX[3], BIT_AT_INDEX[57] | BIT_AT_INDEX[58] | BIT_AT_INDEX[59] };
  const uint64_t CASTLING_KINGSIDE_BLOCK_RELEVANT_AREA[2] = { BIT_AT_INDEX[5] | BIT_AT_INDEX[6], BIT_AT_INDEX[61] | BIT_AT_INDEX[62] };

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
  const uint64_t ZOBRIST_RANDOM_BITMASKS_PLAYERS[2][64] =
  #include "chessData/zobristRandomBitmasksPlayers.in"
  ;
}
