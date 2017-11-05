#pragma once

#include "types.hpp"
#include <array>

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

  PositionValue positionValue;

  void setToStartposition();
  void clearBoard();
  int getNumberHalfmovesPlayed();
  int getNumberMovesPlayed();
  bool whiteToMove();
  bool blackToMove();

  //void printAllBitboards();
  void printPosition();

  static bool cmp(const Position &a, const Position &b)
  {
    return a.positionValue > b.positionValue;
  }
};


#define MAX_NUMBER_MOVES_PER_POSITION 128
class Position;
struct PositionVector
{
  std::array<Position, MAX_NUMBER_MOVES_PER_POSITION> array;
  int size;
};
