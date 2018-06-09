#pragma once

#include "chessTypes.hpp"
#include "chessData.hpp"

using namespace ChessTypes;
using namespace ChessData;

struct Move
{
  size_t from;
  size_t to;
  Piece moved;
  Piece captured;
  Piece promoted;
  uint64_t enPassantCastling;
  bool castled;
  bool capturedEnPassant;
  Score score;
  uint64_t zobristKey;

  std::string getNotation() const
  {
    std::string ret = "";
    ret += getSquareNotation(from);
    ret += getSquareNotation(to);
    if(promoted != NO_PIECE)
    {
      ret += getPieceNotation(BLACK, promoted);
    }
    return ret;
  }

};
