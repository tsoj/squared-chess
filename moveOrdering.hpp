#include "types.hpp"
#include "position.hpp"
#include <algorithm>
#include "bitMasks.hpp"
#include "utils.hpp"
#include <iostream>

using namespace BitMasks;

inline void sortMoves(const COLOR_TYPE & us, const COLOR_TYPE & enemy, const Position & origPosition, PositionVector & positions)
{
  static PositionValue mvvLva_Value; //MVV-LVA (Most Valuable Victim - Least Valuable Aggressor)
  for(int i = 0; i<positions.size; i++)
  {
    positions.array[i].positionValue = 0;
    if(positions.array[i].lastCapturedPieceType != NO_PIECE)
    {

      mvvLva_Value = pieceValues[positions.array[i].lastCapturedPieceType]*10 - pieceValues[positions.array[i].lastMovedPieceType];

      /*if(origPosition.lastPieceMovedToIndex != positions.array[i].lastPieceMovedToIndex)
      {
        positions.array[i].positionValue -= pieceValues[positions.array[i].lastMovedPieceType];
      }*/

      positions.array[i].positionValue = mvvLva_Value;
    }
  }
  std::sort(positions.array.begin(), positions.array.begin() + positions.size, Position::cmp);

}
