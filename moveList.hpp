#pragma once

#include <array>

#include "chessTypes.hpp"
#include "chessData.hpp"
#include "debug.hpp"
#include "position.hpp"
#include "move.hpp"

using namespace ChessTypes;
using namespace ChessData;
using namespace CountZeros;

struct MoveList
{
  private:

  static const size_t MOVE_LIST_MAXIMUM_SIZE = 128;

  size_t currentSize;
  std::array<Move, MOVE_LIST_MAXIMUM_SIZE> movePool;

  public:

  MoveList() : currentSize(0){}
  
  size_t size() const
  {
    return currentSize;
  }

  Move& operator[](const size_t i)
  {
    ASSERT(i < movePool.size())
    ASSERT(i < currentSize)
    return movePool[i];
  }
  const Move& operator[](const size_t i) const
  {
    ASSERT(i < movePool.size())
    ASSERT(i < currentSize)
    return movePool[i];
  }

  void addMove(
    const size_t from,
    const size_t to,
    const Piece moved,
    const Piece captured,
    const Piece promoted,
    const uint64_t enPassantCastling,
    const bool castled,
    const bool capturedEnPassant,
    const Position& origPosition
  )
  {
    currentSize++;
    (*this)[currentSize-1].from = from;
    (*this)[currentSize-1].to = to;
    (*this)[currentSize-1].moved = moved;
    (*this)[currentSize-1].captured = captured;
    (*this)[currentSize-1].promoted = promoted;
    (*this)[currentSize-1].enPassantCastling = enPassantCastling;
    (*this)[currentSize-1].castled = castled;
    (*this)[currentSize-1].capturedEnPassant = capturedEnPassant;
    (*this)[currentSize-1].score = 0;
    (*this)[currentSize-1].zobristKey = origPosition.getUpdatedZobristKey((*this)[currentSize-1]);
  }

  template<Piece piece>
  void generateMoves(const Position& origPosition, const uint64_t newEnPassantCastling, const bool onlyCaptures);
  void generateCastlingMoves(const Position& origPosition, const uint64_t newEnPassantCastling);

};

template<Piece piece>
void MoveList::generateMoves(const Position& origPosition, const uint64_t newEnPassantCastling, const bool onlyCaptures)
{
  ASSERT(piece < NO_PIECE && piece != PAWN);

  const Player enemy = origPosition.enemy;
  const Player us = origPosition.us;
  uint64_t pieceOccupancy = origPosition.pieces[piece] & origPosition.players[us];

  while(pieceOccupancy != 0)
  {
    const size_t from = findAndClearTrailingOne(pieceOccupancy);

    const uint64_t occupancy = origPosition.players[WHITE] | origPosition.players[BLACK];
    uint64_t quietAttackMask = getAttackMask<piece>(from, occupancy);
    uint64_t captureAttackMask = quietAttackMask & origPosition.players[enemy];
    quietAttackMask &= ~captureAttackMask;
    quietAttackMask &= ~origPosition.players[us];

    if(not onlyCaptures)
    {
      while(quietAttackMask != 0)
      {
        const size_t to = findAndClearTrailingOne(quietAttackMask);
        const uint64_t updatedNewEnPassantCastling = newEnPassantCastling & ~BIT_AT_INDEX[from];
        this->addMove(from, to, piece, NO_PIECE, NO_PIECE, updatedNewEnPassantCastling, false, false, origPosition);
      }
    }

    while(captureAttackMask != 0)
    {
      const size_t to = findAndClearTrailingOne(captureAttackMask);
      for(Piece captured = 0; captured<NO_PIECE; captured++)
      {
        if((origPosition.pieces[captured] & BIT_AT_INDEX[to]) != 0)
        {
          const uint64_t updatedNewEnPassantCastling = newEnPassantCastling & ~(BIT_AT_INDEX[to] | BIT_AT_INDEX[from]);
          this->addMove(from, to, piece, captured, NO_PIECE, updatedNewEnPassantCastling, false, false, origPosition);
          break;
        }
      }
    }
  }
}

template<>
void MoveList::generateMoves<PAWN>(const Position& origPosition, const uint64_t newEnPassantCastling, const bool onlyCaptures);





















//
