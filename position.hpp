#pragma once

#include "types.hpp"
#include <array>
#include <iostream>
#include "data.hpp"

using namespace Data;




class Move
{
public:
  Move():
    from(-1),
    to(-1)
  {
  }
  inline bool operator==(const Move & a)
  {
    return (a.from == from && a.to == to);
  }
  void print()
  {
    std::cout << "from: "<<from<<std::endl;
    std::cout << "to: "<<to<<std::endl;
    std::cout << "lastCaptured: "<<lastCaptured<<std::endl;
    std::cout << "lastMoved: "<<lastMoved<<std::endl;
  }
  int from;
  int to;
  PIECE_TYPE lastMoved;
  PIECE_TYPE lastCaptured;
  PIECE_TYPE promotedTo;
};

#define MAX_NUMBER_MOVES_PER_POSITION 128
struct MoveList
{
  inline Move & operator[](int Index)
  {
    return array[Index];
  }
  int size;
private:
  std::array<Move, MAX_NUMBER_MOVES_PER_POSITION> array;
};

class Position
{
public:
  Position();

  Bitboard colors[2];
  Bitboard pieces[6];
  Bitboard enPassant;//if enpassant is possible, one bit will be set where a enemy pawn must capture this pawn
  Bitboard castling;//if castling is allowed then the bit at the rook index that allows castling will be set
  short whoIsToMove; //1 for white, -1 for black to move
  short numberHalfmovesPlayed; //1 for white, -1 for black to move
  Move lastMove;
  Score score;

  void setToStartposition();
  void clearBoard();
  short getNumberHalfmovesPlayed();
  short getNumberMovesPlayed();
  bool whiteToMove();
  bool blackToMove();

  //void printAllBitboards();
  void printPosition();

  static bool cmpScore(const Position &a, const Position &b)
  {
    return a.score > b.score;
  }
};

#define MAX_NUMBER_MOVES_PER_POSITION 128
struct PositionList
{
  inline Position & operator[](int Index)
  {
    return array[Index];
  }
  int size;
private:
  std::array<Position, MAX_NUMBER_MOVES_PER_POSITION> array;
};
