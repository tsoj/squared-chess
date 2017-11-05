#include "search.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>


PositionValue negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth, PositionValue alpha, PositionValue beta)
{
    if(depth<=0)
    {
      return evaluation(us, origPosition);
    }
    static PositionValue currentValue;
    PositionArray newPositions;
    int numberNewPositions;
    generateAllMoves(us, enemy, origPosition, newPositions, numberNewPositions);
    for(int i = 0; i < numberNewPositions; i++)
    {
      currentValue = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
      if(alpha < currentValue)
      {
        alpha = currentValue;
        if(beta <= currentValue)
        {
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

    PositionValue alpha = -POSITION_VALUE_INFINITY;
    PositionValue beta = POSITION_VALUE_INFINITY;
    PositionValue currentValue;
    PositionArray newPositions;
    int bestBoardIndex = 0;
    int numberNewPositions;
    generateAllMoves(us, enemy, origPosition, newPositions, numberNewPositions);
    for(int i = 0; i < numberNewPositions; i++)
    {
      currentValue = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
      if(alpha < currentValue)
      {
        alpha = currentValue;
        bestBoardIndex = i;
      }
    }
    printPosition(newPositions[bestBoardIndex]);
    std::cout << "Evaluation: " << alpha << std::endl;
    return newPositions[bestBoardIndex];
}
