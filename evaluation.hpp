#pragma once

#include "types.hpp"
#include "position.hpp"
#include "data.hpp"
#include "bitOperations.hpp"

using namespace Data;
using namespace BitOperations;

inline Score evaluation(COLOR_TYPE us, Position position)
{
  static Score ret;
  ret = 0;

  static unsigned long fieldIndex;
  static Bitboard tempOccupancy;

  for(int i = QUEEN; i<NO_PIECE; i++)
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
