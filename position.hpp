#pragma once

#include "types.hpp"

class Position
{
public:
  Bitboard whitePieces;
  Bitboard blackPieces;

  Bitboard pawns;
  Bitboard knights;
  Bitboard bishops;
  Bitboard rooks;
  Bitboard queens;
  Bitboard kings;

  uint32 properties;
  /*
  bits 0 - 9: halfmoves played
  bits 10 - 17: row of enpassant
  bit 18 - 19: side of enpassant, both zero if no enpassant move was made
  bit 20: whose move (1 for white, 0 for black to move)
  bits 21 - 24: castling allowed?
  */

  short lastPieceMovedToIndex;
  PIECE_TYPE lastMovedPieceType;
  PIECE_TYPE lastCapturedPieceType;

  void setToStartposition();
  int getNumberHalfmovesPlayed();
  int getNumberMovesPlayed();
  bool whiteToMove();
  bool blackToMove();
};
