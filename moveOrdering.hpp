#include "types.hpp"
#include "position.hpp"
#include <algorithm>
#include "data.hpp"
#include "utils.hpp"
#include <iostream>

using namespace Data;
inline void sortMoves(const COLOR_TYPE & us, const COLOR_TYPE & enemy, const Position & origPosition, PositionList & newPositions, Move killerMove[2])
{
  for(int i = 0; i<newPositions.size; i++)
  {

    newPositions[i].score = 0;
    if(newPositions[i].lastMove.lastCaptured != NO_PIECE)
    {
      newPositions[i].score += pieceValues[newPositions[i].lastMove.lastCaptured]*10 - pieceValues[newPositions[i].lastMove.lastMoved] + rankValues[newPositions[i].lastMove.to / 8][us];
      if(newPositions[i].lastMove.to == origPosition.lastMove.to)
      {
        newPositions[i].score += pieceValues[newPositions[i].lastMove.lastCaptured];
      }
    }
    else if(newPositions[i].lastMove == killerMove[0])
    {
      newPositions[i].score += 150;
    }
    else if(newPositions[i].lastMove == killerMove[1])
    {
      newPositions[i].score += 100;
    }
    newPositions[i].score += pieceValues[newPositions[i].lastMove.promotedTo]*10;
  }
  std::sort(&newPositions[0], &newPositions[newPositions.size], Position::cmpScore);

}
