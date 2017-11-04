#include "search.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>

PositionValue negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth)
{
  if(depth<=0)
  {
    return evaluation(us, origPosition);
  }
  static PositionValue ret;
  static PositionValue currentValue;

  ret = -POSITION_VALUE_INFINITY;
  std::vector<Position> newPositions;
  generateAllMoves(us, enemy, origPosition, newPositions);
  for(int i = 0; i < newPositions.size(); i++)
  {
    currentValue = -negaMax(enemy, us, newPositions[i], depth - 1);
    if(ret<currentValue)
    {
      ret = currentValue;
    }
  }
  return ret;
}
