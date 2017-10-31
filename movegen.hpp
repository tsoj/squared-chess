#pragma once

#include <vector>
#include "position.hpp"
#include "movegen.hpp"
#include "bitMasks.hpp"
#include "types.hpp"
#include "bitOperations.hpp"
#include <iostream>

using namespace BitOperations;
using namespace BitMasks;

template<PIECE_TYPE Pt, COLOR_TYPE us, COLOR_TYPE enemy>
void generateMoves(const Position & origPosition, const Bitboard & occupancy, std::vector<Position> & newPositions)
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
          newPosition.lastCapturedPieceType = NO_PIECE;
          newPosition.colors[us] = (origPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
          newPosition.pieces[Pt] = (origPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = Pt;
          newPosition.lastPieceMovedToIndex = toFieldIndex;
          newPosition.lastPieceMovedFromIndex = fromFieldIndex;
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
          for(int i = 0; i<6; i+=1)
          {
            if(origPosition.pieces[i] & bitAtIndex[toFieldIndex])
            {
              newPosition.lastCapturedPieceType = PIECE_TYPE(i);
              newPosition.colors[enemy] = origPosition.colors[enemy] & ~bitAtIndex[toFieldIndex];
              newPosition.pieces[PIECE_TYPE(i)] = origPosition.colors[PIECE_TYPE(i)] & ~bitAtIndex[toFieldIndex];
            }
          }
          newPosition.colors[us] = (origPosition.colors[us] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
          newPosition.pieces[Pt] = (origPosition.pieces[Pt] & ~bitAtIndex[fromFieldIndex]) | bitAtIndex[toFieldIndex];
          newPosition.lastMovedPieceType = Pt;
          newPosition.lastPieceMovedToIndex = toFieldIndex;
          newPosition.lastPieceMovedFromIndex = fromFieldIndex;
          newPositions.push_back(newPosition);
          captureAttackMask &= ~bitAtIndex[toFieldIndex];
        } while(captureAttackMask);
      }
      pieceOccupancy &= ~bitAtIndex[fromFieldIndex];
    } while(pieceOccupancy);
  }
}
