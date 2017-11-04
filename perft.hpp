#pragma once

#include <vector>
#include "types.hpp"
#include "movegen.hpp"
#include "utils.hpp"

using namespace BitMasks;
using namespace BitOperations;

uint64 perft(int depth, const Position & origPosition)
{
  static COLOR_TYPE us;
  static COLOR_TYPE enemy;
  if(origPosition.whoIsToMove == WHITE_TO_MOVE)
  {
    us = WHITE;
    enemy = BLACK;
  }
  else
  {
    us = BLACK;
    enemy = WHITE;
  }
  std::vector<Position> newPositions;
  uint64 nodes = 0;
  if (depth == 0)
  {
    return 1;
  }
  generateAllMoves(us, enemy, origPosition, newPositions);
  for (int i = 0; i < newPositions.size(); i++) {
    /*if(isKingInCheck(us, enemy, newPositions[i]))
    {
      if(depth == 1)
      {
        //nodes++;
      }
      continue;
    }*/
    nodes += perft(depth - 1, newPositions[i]);
  }
  return nodes;
}
