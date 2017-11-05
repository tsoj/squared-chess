#include "search.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>

/*PositionValue negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth, PositionValue alpha, PositionValue beta)
{
  if(depth<=0)
  {
    return evaluation(us, origPosition);
  }
  static PositionValue ret;
  static PositionValue currentValue;
  ret = alpha;
  std::vector<Position> newPositions;
  generateAllMoves(us, enemy, origPosition, newPositions);
  for(int i = 0; i < newPositions.size(); i++)
  {
    currentValue = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
    if(ret < currentValue)
    {
      ret = currentValue;
    }
  }
  return ret;
}
void startSearch(Position origPosition, int depth)
{
  COLOR_TYPE us;
  COLOR_TYPE enemy;
  PositionValue alpha;
  PositionValue beta;

  if(origPosition.whoIsToMove == WHITE_TO_MOVE)
  {
    us = WHITE;
    enemy = BLACK;
    alpha = -POSITION_VALUE_INFINITY;
    beta = POSITION_VALUE_INFINITY;
  }
  else
  {
    us = BLACK;
    enemy = WHITE;
    alpha = POSITION_VALUE_INFINITY;
    beta = -POSITION_VALUE_INFINITY;
  }

  PositionValue ret = alpha;
  PositionValue currentValue;
  std::vector<Position> newPositions;
  int bestBoardIndex = 0;
  generateAllMoves(us, enemy, origPosition, newPositions);
  for(int i = 0; i < newPositions.size(); i++)
  {
    currentValue = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
    std::cout << currentValue << std::endl;
    if(ret < currentValue)
    {
      ret = currentValue;
      bestBoardIndex = i;
    }
  }

  printPosition(newPositions[bestBoardIndex]);
  std::cout << "Evaluation: " << ret << std::endl;
}
*/

PositionValue minMaxBlack(Position origPosition, int depth, PositionValue alpha, PositionValue beta);
PositionValue minMaxWhite(Position origPosition, int depth, PositionValue alpha, PositionValue beta)
{
    if(depth<=0)
    {
      return evaluation(WHITE, origPosition);
    }
    static PositionValue currentValue;
    PositionArray newPositions;
    int numberNewPositions;
    generateAllMoves(WHITE, BLACK, origPosition, newPositions, numberNewPositions);
    for(int i = 0; i < numberNewPositions; i++)
    {
      currentValue = minMaxBlack(newPositions[i], depth - 1, beta, alpha);
      if(alpha < currentValue)
      {
        alpha = currentValue;

        if(beta <= currentValue)
        {
          return alpha;
        }
      }
    }
    return alpha;
}
PositionValue minMaxBlack(Position origPosition, int depth, PositionValue alpha, PositionValue beta)
{
    if(depth<=0)
    {
      return evaluation(WHITE, origPosition);
    }
    static PositionValue currentValue;
    PositionArray newPositions;
    int numberNewPositions;
    generateAllMoves(BLACK, WHITE, origPosition, newPositions, numberNewPositions);
    for(int i = 0; i < numberNewPositions; i++)
    {
      currentValue = minMaxWhite(newPositions[i], depth - 1, beta, alpha);
      if(alpha > currentValue)
      {
        alpha = currentValue;

        if(beta >= currentValue)
        {
          return alpha;
        }
      }
    }
    return alpha;
}
Position startSearchMinMax(Position origPosition, int depth)
{
  PositionValue ret;
  PositionValue currentValue;
  PositionArray newPositions;
  int bestBoardIndex = 0;
  int numberNewPositions;
  if(origPosition.whoIsToMove == WHITE_TO_MOVE)
  {
    ret = -POSITION_VALUE_INFINITY;
    generateAllMoves(WHITE, BLACK, origPosition, newPositions, numberNewPositions);
    for(int i = 0; i < numberNewPositions; i++)
    {
      currentValue = minMaxBlack(newPositions[i], depth - 1, POSITION_VALUE_INFINITY, ret);
      if(ret < currentValue)
      {
        ret = currentValue;
        bestBoardIndex = i;
      }
    }
  }
  else
  {
    ret = POSITION_VALUE_INFINITY;
    generateAllMoves(BLACK, WHITE, origPosition, newPositions, numberNewPositions);
    for(int i = 0; i < numberNewPositions; i++)
    {
      currentValue = minMaxWhite(newPositions[i], depth - 1, -POSITION_VALUE_INFINITY, ret);
      if(ret > currentValue)
      {
        ret = currentValue;
        bestBoardIndex = i;
      }
    }
  }
  printPosition(newPositions[bestBoardIndex]);
  std::cout << "Evaluation: " << ret << std::endl;
  return newPositions[bestBoardIndex];
}
