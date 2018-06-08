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
  const size_t offset;

  static std::vector<Move> movePool;
  static std::vector<size_t> freeOffsets;

  public:

  MoveList() : offset(freeOffsets.back())
  {
    freeOffsets.pop_back();
    if(freeOffsets.size() == 0)
    {
      movePool.resize(offset+MOVE_LIST_MAXIMUM_SIZE);
      freeOffsets.push_back(movePool.size());
    }
    size = 0;
  }
  ~MoveList()
  {
    freeOffsets.push_back(offset);
  }

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
    const size_t from,
    const size_t to,
    const Piece moved,
    const Piece captured,
    const Piece promoted,
    const uint64_t enPassantCastling,
    const bool castled,
    const bool capturedEnPassant/*,
    const Position& origPosition*/
  )
  {
    size++;
    (*this)[size-1].from = from;
    (*this)[size-1].to = to;
    (*this)[size-1].moved = moved;
    (*this)[size-1].captured = captured;
    (*this)[size-1].promoted = promoted;
    (*this)[size-1].enPassantCastling = enPassantCastling;
    (*this)[size-1].castled = castled;
    (*this)[size-1].capturedEnPassant = capturedEnPassant;
    (*this)[size-1].score = 0;
    //TODO: ? zobristKey
  }

  template<Piece p>
  void generateMoves(const Position& origPosition, uint64_t newEnPassantCastling, bool onlyCaptures)
  {
    ASSERT(false);
  }
  void generateCastlingMoves(const Position& origPosition, uint64_t newEnPassantCastling);

};
template<>
void MoveList::generateMoves<PAWN>(const Position& origPosition, uint64_t newEnPassantCastling, bool onlyCaptures);
template<>
void MoveList::generateMoves<KNIGHT>(const Position& origPosition, uint64_t newEnPassantCastling, bool onlyCaptures);
template<>
void MoveList::generateMoves<BISHOP>(const Position& origPosition, uint64_t newEnPassantCastling, bool onlyCaptures);
template<>
void MoveList::generateMoves<ROOK>(const Position& origPosition, uint64_t newEnPassantCastling, bool onlyCaptures);
template<>
void MoveList::generateMoves<QUEEN>(const Position& origPosition, uint64_t newEnPassantCastling, bool onlyCaptures);
template<>
void MoveList::generateMoves<KING>(const Position& origPosition, uint64_t newEnPassantCastling, bool onlyCaptures);





















//
