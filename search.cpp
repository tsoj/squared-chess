#include "search.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>
#include "moveOrdering.hpp"
#include <chrono>
#define MAX_DEPTH 64

int nodes;
Move killerMove[MAX_DEPTH][2];

inline Score negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth, Score alpha, Score beta)
{
  if(depth<=0)
  {
    return evaluation(us, origPosition);
  }
  nodes++;
  static Score currentValue;
  static Move localKillerMove;
  PositionList newPositions;
  generateAllMoves(us, enemy, origPosition, newPositions);
  sortMoves(us, enemy, origPosition, newPositions, killerMove[depth]);
  for(int i = 0; i < newPositions.size; i++)
  {
    if(isKingInCheck(us, enemy, newPositions[i]))
    {
      continue;
    }
    currentValue = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
    if(alpha < currentValue)
    {
      alpha = currentValue;
      if(beta <= currentValue)
      {
        killerMove[depth][1] = killerMove[depth][0];
        killerMove[depth][0] = newPositions[i].lastMove;
        break;
      }
    }
  }
  //check for MATE or STALEMATE
  if(alpha == -SCORE_INFINITY)
  {
    if(isKingInCheck(us, enemy, origPosition))
    {
      alpha -= depth;
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
  Score alpha = -SCORE_INFINITY;
  Score beta = SCORE_INFINITY;
  Score currentValue;
  PositionList newPositions;
  int bestBoardIndex = 0;
  generateAllMoves(us, enemy, origPosition, newPositions);
  sortMoves(us, enemy, origPosition, newPositions, killerMove[depth]);
  for(int i = 0; i < newPositions.size; i++)
  {
    currentValue = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
    if(alpha < currentValue)
    {
      alpha = currentValue;
      bestBoardIndex = i;
    }
    printPosition(newPositions[i]);
    std::cout << "Value: " << currentValue << std::endl << std::endl;
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  printPosition(newPositions[bestBoardIndex]);
  newPositions[bestBoardIndex].lastMove.print();
  std::cout << "Score: " << alpha << std::endl;
  std::cout << "Time elapsed: " << timeElapsed << std::endl;
  std::cout << "Nodes: " << nodes << std::endl;
  std::cout << "Nodes/Second: " << nodes / (0.001 * timeElapsed) << std::endl;

  return newPositions[bestBoardIndex];
}
