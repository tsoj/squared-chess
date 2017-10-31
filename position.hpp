#pragma once

#include "types.hpp"

class Position
{
public:
  Position();

  Bitboard colors[2];
  Bitboard pieces[6];
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
  void clearBoard();
  int getNumberHalfmovesPlayed();
  int getNumberMovesPlayed();
  bool whiteToMove();
  bool blackToMove();
};
