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
  bit 20: whose move (1 for white, 0 for black to move)
  bits 21 - 24: castling allowed?
  */
  Bitboard enPassant;
  //if enpassant is possible, one bit will be set where a enemy pawn must capture this pawn

  int lastPieceMovedToIndex;
  int lastPieceMovedFromIndex;
  PIECE_TYPE lastMovedPieceType;
  PIECE_TYPE lastCapturedPieceType;

  void setToStartposition();
  void clearBoard();
  int getNumberHalfmovesPlayed();
  int getNumberMovesPlayed();
  bool whiteToMove();
  bool blackToMove();

  //void printAllBitboards();
};
