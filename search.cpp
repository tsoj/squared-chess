#include "search.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>
#include "moveOrdering.hpp"
#include <chrono>

int nodes;

inline PositionValue negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth, PositionValue alpha, PositionValue beta, Move & killerMove, Move & nextkillerMove)
{
  if(depth<=0)
  {
    return evaluation(us, origPosition);
  }
  nodes++;
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
  if(alpha == -POSITION_VALUE_INFINITY)
  {
    if(isKingInCheck(us, enemy, origPosition))
    {
      alpha += depth;
    }
    else
    {
      alpha = 0;      
    }
  }
  return alpha;
}

Position startSearch(Position origPosition, int depth)
{
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
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
  nodes = 0;
  static Move localKillerMove;
  static Move nextkillerMove;
  PositionValue alpha = -POSITION_VALUE_INFINITY;
  PositionValue beta = POSITION_VALUE_INFINITY;
  PositionValue currentValue;
  PositionVector newPositions;
  int bestBoardIndex = 0;
  int numberLegalMoves = 0;
  generateAllMoves(us, enemy, origPosition, newPositions);
  for(int i = 0; i < newPositions.size; i++)
  {
    if(isKingInCheck(us, enemy, newPositions.array[i]))
    {
      continue;
    }
    numberLegalMoves++;
    currentValue = -negaMax(enemy, us, newPositions.array[i], depth - 1, -beta, -alpha, localKillerMove, nextkillerMove);
    if(alpha < currentValue)
    {
      alpha = currentValue;
      bestBoardIndex = i;
    }
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  if(numberLegalMoves == 0)
  {
    std::cout << std::endl << "MATE" << std::endl << std::endl;
  }
  else{
    printPosition(newPositions.array[bestBoardIndex]);
  }
  newPositions.array[bestBoardIndex].lastMove.print();
  std::cout << "Evaluation: " << alpha << std::endl;
  std::cout << "Time elapsed: " << timeElapsed << std::endl;
  std::cout << "Nodes: " << nodes << std::endl;
  std::cout << "Nodes/Second: " << nodes / (0.001 * timeElapsed) << std::endl;

  return newPositions.array[bestBoardIndex];
}
