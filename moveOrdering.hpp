#include "types.hpp"
#include "position.hpp"
#include <algorithm>
#include "bitMasks.hpp"
#include "utils.hpp"
#include <iostream>

using namespace BitMasks;
inline void sortMoves(const COLOR_TYPE & us, const COLOR_TYPE & enemy, const Position & origPosition, PositionVector & newPositions, Move killerMove[2])
{
  for(int i = 0; i<newPositions.size; i++)
  {

    newPositions.array[i].positionValue = 0;
    if(newPositions.array[i].lastMove.lastCaptured != NO_PIECE)
    {
      newPositions.array[i].positionValue += pieceValues[newPositions.array[i].lastMove.lastCaptured]*10 - pieceValues[newPositions.array[i].lastMove.lastMoved] + rankValues[newPositions.array[i].lastMove.to / 8][us];
      if(newPositions.array[i].lastMove.to == origPosition.lastMove.to)
      {
        newPositions.array[i].positionValue += pieceValues[newPositions.array[i].lastMove.lastCaptured];
      }
    }
    else if(newPositions.array[i].lastMove == killerMove[0])
    {
      newPositions.array[i].positionValue += 150;
    }
    else if(newPositions.array[i].lastMove == killerMove[1])
    {
      newPositions.array[i].positionValue += 100;
    }
    newPositions.array[i].positionValue += pieceValues[newPositions.array[i].lastMove.promotedTo]*10;
  }
  std::sort(newPositions.array.begin(), newPositions.array.begin() + newPositions.size, Position::cmpValue);

}
