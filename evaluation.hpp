#pragma once

#include "types.hpp"
#include "position.hpp"
#include "bitMasks.hpp"
#include "bitOperations.hpp"

using namespace BitMasks;
using namespace BitOperations;

inline PositionValue evaluation(COLOR_TYPE us, Position position)
{
  static PositionValue ret;
  ret = 0;

  static unsigned long fieldIndex;
  static Bitboard tempOccupancy;

  for(int i = 0; i<NO_PIECE; i++)
  {
    if(position.pieces[i])
    {
      tempOccupancy = position.pieces[i];
      do
      {
        bitScanForward(fieldIndex, tempOccupancy);
        if(bitAtIndex[fieldIndex] & position.colors[us])
        {
          ret += pieceValues[i];
        }
        else{
          ret -= pieceValues[i];
        }
        tempOccupancy &= ~bitAtIndex[fieldIndex];
      } while(tempOccupancy);
    }
  }

  return ret;
}
