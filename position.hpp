#pragma once

#include "types.hpp"

class Position
{
public:
  Position();

  Bitboard colors[2];
  Bitboard pieces[6];
  Bitboard enPassant;//if enpassant is possible, one bit will be set where a enemy pawn must capture this pawn
  Bitboard castling[2];//if castling is allowed then the bit at the rook index that allows castling will be set
  int whoIsToMove; //1 for white, -1 for black to move
  int numberHalfmovesPlayed; //1 for white, -1 for black to move

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
  void printPosition();
};
