#pragma once

#include <vector>

#include "chessTypes.hpp"
#include "chessData.hpp"
#include "debug.hpp"
#include "position.hpp"

using namespace ChessTypes;
using namespace ChessData;
using namespace CountZeros;


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
};

struct MoveList
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
    ASSERT(offset+MOVE_LIST_MAXIMUM_SIZE <= movePool.size());
    size = 0;
  }
  ~MoveList()
  {
    freeOffsets.push_back(offset);
  }

  Move& operator[](const size_t i)
  {
    ASSERT(i+offset < MOVE_LIST_MAXIMUM_SIZE)
    ASSERT(i+offset < size)
    return movePool[i+offset];
  }
  const Move& operator[](const size_t i) const
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
    const bool capturedEnPassant,
    const Position& origPosition
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
    (*this)[size-1].zobristKey = origPosition.getUpdatedZobristKey((*this)[size-1]);
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
  if(pieceOccupancy != 0)
  {

    while(pieceOccupancy != 0)
    {
      const size_t from = findAndClearTrailingOne(pieceOccupancy);

      const uint64_t occupancy = origPosition.players[WHITE] | origPosition.players[BLACK];
      uint64_t quietAttackMask = getAttackMask<piece>(from, occupancy);
      uint64_t captureAttackMask = quietAttackMask & origPosition.players[enemy];
      quietAttackMask &= !captureAttackMask;
      quietAttackMask &= !origPosition.players[us];

      if(quietAttackMask != 0 && not onlyCaptures)
      {
        while(quietAttackMask != 0)
        {
          const size_t to = findAndClearTrailingOne(quietAttackMask);
          const uint64_t updatedNewEnPassantCastling = newEnPassantCastling & ~BIT_AT_INDEX[from];
          this->addMove(from, to, piece, NO_PIECE, NO_PIECE, updatedNewEnPassantCastling, false, false, origPosition);
        }
      }

      if(captureAttackMask != 0)
      {
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

  }
}

template<>
void MoveList::generateMoves<PAWN>(const Position& origPosition, const uint64_t newEnPassantCastling, const bool onlyCaptures);





















//
