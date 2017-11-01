#pragma once

#include <vector>
#include "position.hpp"
#include "movegen.hpp"
#include "bitMasks.hpp"
#include "types.hpp"
#include "bitOperations.hpp"

using namespace BitOperations;
using namespace BitMasks;

template<PIECE_TYPE Pt>
void applyQuietMove(
  COLOR_TYPE us, COLOR_TYPE enemy,
  const Position & origPosition,
  Position & newPosition,
  int toFieldIndex, int fromFieldIndex
)

{
  newPosition.lastCapturedPieceType = NO_PIECE;
  newPosition.colors[us] = (origPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[Pt] = (origPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMovedPieceType = Pt;
  newPosition.lastPieceMovedToIndex = toFieldIndex;
  newPosition.lastPieceMovedFromIndex = fromFieldIndex;
}
template<PIECE_TYPE Pt>
void applyCaptureMove(
  COLOR_TYPE us, COLOR_TYPE enemy,
  const Position & origPosition,
  Position newPosition,
  int toFieldIndex, int fromFieldIndex
)
{
  for(int i = 0; i<6; i+=1)
  {
    if(origPosition.pieces[i] & bitAtIndex[toFieldIndex])
    {
      newPosition.lastCapturedPieceType = PIECE_TYPE(i);
      newPosition.colors[enemy] = origPosition.colors[enemy] & ~bitAtIndex[toFieldIndex];
      newPosition.pieces[PIECE_TYPE(i)] = origPosition.pieces[PIECE_TYPE(i)] & ~bitAtIndex[toFieldIndex];
    }
  }
  newPosition.colors[us] = (origPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[Pt] = (origPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMovedPieceType = Pt;
  newPosition.lastPieceMovedToIndex = toFieldIndex;
  newPosition.lastPieceMovedFromIndex = fromFieldIndex;
}

void applyCaptureEnPassantMove(
  COLOR_TYPE us, COLOR_TYPE enemy,
  const Position & origPosition,
  Position & newPosition,
  int toFieldIndex, int fromFieldIndex,
  Bitboard enPassant,
  int moveDirection
)
{
  newPosition.lastCapturedPieceType = PAWN;
  newPosition.colors[enemy] = origPosition.colors[enemy] & ~bitAtIndex[toFieldIndex - moveDirection];
  newPosition.pieces[PAWN] = origPosition.pieces[PAWN] & ~bitAtIndex[toFieldIndex - moveDirection];
  newPosition.colors[us] = (origPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.pieces[PAWN] = (origPosition.pieces[PAWN] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
  newPosition.lastMovedPieceType = PAWN;
  newPosition.lastPieceMovedToIndex = toFieldIndex;
  newPosition.lastPieceMovedFromIndex = fromFieldIndex;
}

template<PIECE_TYPE Pt>
void generateMoves(
  COLOR_TYPE us, COLOR_TYPE enemy,
  const Position & origPosition,
  const Bitboard & occupancy,
  std::vector<Position> & newPositions
)
{
  static Bitboard pieceOccupancy;
  static Bitboard quietAttackMask;
  static Bitboard captureAttackMask;
  static unsigned long fromFieldIndex;
  static unsigned long toFieldIndex;
  static Position newPosition;

  pieceOccupancy = origPosition.pieces[Pt] & origPosition.colors[us];
  if(pieceOccupancy)
  {
    do
    {
      bitScanForward(fromFieldIndex, pieceOccupancy);
      quietAttackMask = getAttackBitboard<Pt>(occupancy, fromFieldIndex) & ~origPosition.colors[us];
      captureAttackMask = quietAttackMask & origPosition.colors[enemy];
      quietAttackMask &= ~captureAttackMask;
      if(quietAttackMask)
      {
        do
        {
          bitScanForward(toFieldIndex, quietAttackMask);
          newPosition = origPosition;
          applyQuietMove<Pt>(us, enemy, origPosition, newPosition, toFieldIndex, fromFieldIndex);
          newPositions.push_back(newPosition);
          quietAttackMask &= ~bitAtIndex[toFieldIndex];
        } while(quietAttackMask);
      }
      if(captureAttackMask)
      {
        do
        {
          bitScanForward(toFieldIndex, captureAttackMask);
          newPosition = origPosition;
          applyCaptureMove<Pt>(us, enemy, origPosition, newPosition, toFieldIndex, fromFieldIndex);
          newPositions.push_back(newPosition);
          captureAttackMask &= ~bitAtIndex[toFieldIndex];
        } while(captureAttackMask);
      }
      pieceOccupancy &= ~bitAtIndex[fromFieldIndex];
    } while(pieceOccupancy);
  }
}

void generatePawnMoves(
  COLOR_TYPE us, COLOR_TYPE enemy,
  const Position & origPosition,
  std::vector<Position> & newPositions,
  Bitboard enPassant
)
{
  static Bitboard pawnOccupancy;
  static Bitboard pawnDoubleMove;
  static unsigned long fromFieldIndex;
  static unsigned long toFieldIndex;
  static Position newPosition;
  static int moveDirection;

  if(us == WHITE)
  {
    moveDirection = 8;
    pawnDoubleMove = ranks[1];
  }
  else
  {
    moveDirection = -8;
    pawnDoubleMove = ranks[6];
  }

  pawnOccupancy = origPosition.pieces[PAWN] & origPosition.colors[us];

  do
  {
    bitScanForward(fromFieldIndex, pawnOccupancy);
    toFieldIndex = fromFieldIndex + moveDirection;
    if(!((origPosition.colors[BLACK] | origPosition.colors[WHITE]) & bitAtIndex[toFieldIndex]))
    {
      newPosition = origPosition;
      applyQuietMove<PAWN>(us, enemy, origPosition, newPosition, toFieldIndex, fromFieldIndex);
      newPositions.push_back(newPosition);

      toFieldIndex += moveDirection;
      if(
        !((origPosition.colors[BLACK] | origPosition.colors[WHITE]) & bitAtIndex[toFieldIndex]) &&
        (bitAtIndex[fromFieldIndex] & pawnDoubleMove)
      )
      {
        newPosition = origPosition;
        newPosition.enPassant = bitAtIndex[fromFieldIndex - moveDirection];
        applyQuietMove<PAWN>(us, enemy, origPosition, newPosition, toFieldIndex, fromFieldIndex);
        newPositions.push_back(newPosition);
      }
    }
    toFieldIndex = fromFieldIndex + moveDirection + EAST;
    if((origPosition.colors[BLACK] | enPassant) & bitAtIndex[toFieldIndex])
    {
      if(enPassant & bitAtIndex[toFieldIndex])
      {
        newPosition = origPosition;
        applyCaptureEnPassantMove(
          us, enemy,
          origPosition,
          newPosition,
          toFieldIndex,
          fromFieldIndex,
          enPassant,
          moveDirection
        );
        newPositions.push_back(newPosition);
      }
      else
      {
        newPosition = origPosition;
        applyCaptureMove<PAWN>(us, enemy, origPosition, newPosition, toFieldIndex, fromFieldIndex);
        newPositions.push_back(newPosition);
      }
    }
    toFieldIndex = fromFieldIndex + moveDirection + WEST;
    if((origPosition.colors[BLACK] | enPassant) & bitAtIndex[toFieldIndex])
    {
      if(enPassant & bitAtIndex[toFieldIndex])
      {
        newPosition = origPosition;
        applyCaptureEnPassantMove(
          us, enemy,
          origPosition,
          newPosition,
          toFieldIndex,
          fromFieldIndex,
          enPassant,
          moveDirection
        );
        newPositions.push_back(newPosition);
      }
      else
      {
        newPosition = origPosition;
        applyCaptureMove<PAWN>(us, enemy, origPosition, newPosition, toFieldIndex, fromFieldIndex);
        newPositions.push_back(newPosition);
      }
    }
    pawnOccupancy &= ~bitAtIndex[fromFieldIndex];
  } while(pawnOccupancy);
}
