#include "search.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>
#include "moveOrdering.hpp"

inline PositionValue negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth, PositionValue alpha, PositionValue beta, Move & killerMove, Move & nextkillerMove)
{
    if(depth<=0)
    {
      return evaluation(us, origPosition);
    }
    static PositionValue currentValue;
    static Move localKillerMove;
    PositionVector newPositions;
    generateAllMoves(us, enemy, origPosition, newPositions);
    sortMoves(us, enemy, origPosition, newPositions, killerMove);
    for(int i = 0; i < newPositions.size; i++)
    {
      if(isKingInCheck(us, enemy, newPositions.array[i]))
      {
        continue;
      }
      currentValue = -negaMax(enemy, us, newPositions.array[i], depth - 1, -beta, -alpha, nextkillerMove, localKillerMove);
      if(alpha < currentValue)
      {
        alpha = currentValue;
        if(beta <= currentValue)
        {
          killerMove = newPositions.array[i].lastMove;
          break;
        }
      }
    }
    return alpha;
}

Position startSearch(Position origPosition, int depth)
{
    COLOR_TYPE us;
    COLOR_TYPE enemy;

    if(origPosition.whoIsToMove == WHITE_TO_MOVE)
    {
      us = WHITE;
      enemy = BLACK;
    }
    else
    {
      us = BLACK;
      enemy = WHITE;;
    }

    static Move localKillerMove;
    static Move nextkillerMove;
    PositionValue alpha = -POSITION_VALUE_INFINITY;
    PositionValue beta = POSITION_VALUE_INFINITY;
    PositionValue currentValue;
    PositionVector newPositions;
    int bestBoardIndex = 0;
    generateAllMoves(us, enemy, origPosition, newPositions);
    for(int i = 0; i < newPositions.size; i++)
    {
      currentValue = -negaMax(enemy, us, newPositions.array[i], depth - 1, -beta, -alpha, localKillerMove, nextkillerMove);
      if(alpha < currentValue)
      {
        alpha = currentValue;
        bestBoardIndex = i;
      }
    }
    printPosition(newPositions.array[bestBoardIndex]);
    std::cout << "Evaluation: " << alpha << std::endl;
    return newPositions.array[bestBoardIndex];
}
