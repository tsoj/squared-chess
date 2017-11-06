#include "types.hpp"
#include "position.hpp"
#include <algorithm>
#include "bitMasks.hpp"
#include "utils.hpp"
#include <iostream>

using namespace BitMasks;
inline void sortMoves(const COLOR_TYPE & us, const COLOR_TYPE & enemy, const Position & origPosition, PositionVector & newPositions, const Move & killerMove)
{
  for(int i = 0; i<newPositions.size; i++)
  {

    newPositions.array[i].positionValue = 0;
    if(newPositions.array[i].lastMove.lastCaptured != NO_PIECE)
    {
      newPositions.array[i].positionValue += pieceValues[newPositions.array[i].lastMove.lastCaptured]*10 - pieceValues[newPositions.array[i].lastMove.lastMoved];
    }
    else if(newPositions.array[i].lastMove == killerMove)
    {
      newPositions.array[i].positionValue += 10;
      //std::cout << ++o <<std::endl;
    }
    newPositions.array[i].positionValue += pieceValues[newPositions.array[i].lastMove.promotedTo];
  }
  std::sort(newPositions.array.begin(), newPositions.array.begin() + newPositions.size, Position::cmpValue);

}
