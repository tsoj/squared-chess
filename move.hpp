#pragma once

#include <vector>

#include "chessTypes.hpp"
#include "chessData.hpp"
#include "debug.hpp"
#include "position.hpp"

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
};


class MoveList
{
  private:

  static const size_t MOVE_LIST_MAXIMUM_SIZE = 128;

  size_t size;
  size_t offset;

  static std::vector<Move> movePool;
  static std::vector<size_t> freeOffsets;

  public:

  /*MoveList()
  {
    if(freeOffsets.size() > 0)
    {
      offset = freeOffsets.back();
      freeOffsets.pop_back();
    }
    else
    {
      offset = movePool.size();
      movePool.resize(offset+MOVE_LIST_MAXIMUM_SIZE);
    }
    size = 0;

    std::cout << "hi" << std::endl;
  }
  ~MoveList()
  {
    //freeOffsets.push_back(offset);
  }
  */
  Move& operator[](size_t i)
  {
    ASSERT(i+offset < MOVE_LIST_MAXIMUM_SIZE)
    ASSERT(i+offset < size)
    return movePool[i+offset];
  }
  const Move& operator[](size_t i) const
  {
    ASSERT(i+offset < MOVE_LIST_MAXIMUM_SIZE)
    ASSERT(i+offset < size)
    return movePool[i+offset];
  }

  void addMove(
    size_t from,
    size_t to,
    Piece moved,
    Piece captured,
    Piece promoted,
    uint64_t enPassantCastling,
    bool castled,
    bool capturedEnPassant,
    const Position& origPosition
  )
  {
    (*this)[size].from = from;
    (*this)[size].to = to;
    (*this)[size].moved = moved;
    (*this)[size].captured = captured;
    (*this)[size].promoted = promoted;
    (*this)[size].enPassantCastling = enPassantCastling;
    (*this)[size].castled = castled;
    (*this)[size].capturedEnPassant = capturedEnPassant;
    (*this)[size].score = 0;
    //TODO: ? zobristKey
    size++;
  }

  template<Piece p>
  void generateMoves()
  {
    ASSERT(false);
  }
};





















//
