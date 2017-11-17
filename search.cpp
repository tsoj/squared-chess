#include "search.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>
#include "moveOrdering.hpp"
#include <chrono>
#include "zobrist.hpp"
#define MAX_DEPTH 64
#define TP_TABLE_SIZE 10000000

struct TranspositionTableContent
{
  Bitboard zobristKey;
  int searchedDepth;
  Score searchedScore;
};

int Perft(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth)
{
    int nodes = 0;

    if (depth == 0) return 1;

    PositionList newPositions;
    generateAllMoves(us, enemy, origPosition, newPositions);
    for (int i = 0; i < newPositions.size; i++) {
        if (!isKingInCheck(us, enemy, newPositions[i]))
            nodes += Perft(enemy, us, newPositions[i], depth - 1);
    }
    return nodes;
}

void initTranspositionTable(TranspositionTableContent ret[TP_TABLE_SIZE])
{
  for (int i = 0; i < TP_TABLE_SIZE; i++)
  {
    ret[i].zobristKey = 0;
    ret[i].searchedDepth = -1;
    ret[i].searchedScore = 0;
  }
}

int nodes;
Move killerMove[MAX_DEPTH][2];
TranspositionTableContent tpTable[TP_TABLE_SIZE];

inline Score negaMax(COLOR_TYPE us, COLOR_TYPE enemy, Position origPosition, int depth, Score alpha, Score beta)
{
  if(depth<=0)
  {
    return evaluation(us, enemy, origPosition);
  }
  nodes++;
  Score currentScore;
  Bitboard currentZobristKey;
  int numberLegalMoves = 0;
  PositionList newPositions;
  generateAllMoves(us, enemy, origPosition, newPositions);
  sortMoves(us, enemy, origPosition, newPositions, killerMove[depth]);
  for(int i = 0; i < newPositions.size; i++)
  {
    if(isKingInCheck(us, enemy, newPositions[i]))
    {
      continue;
    }
    numberLegalMoves++;
    currentZobristKey = getZobristKey(newPositions[i]);

    if((tpTable[currentZobristKey%TP_TABLE_SIZE].zobristKey == currentZobristKey) && (tpTable[currentZobristKey%TP_TABLE_SIZE].searchedDepth >= depth))
    {
      currentScore = tpTable[currentZobristKey%TP_TABLE_SIZE].searchedScore;
    }
    else
    {
      currentScore = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
      tpTable[currentZobristKey%TP_TABLE_SIZE].zobristKey = currentZobristKey;
      tpTable[currentZobristKey%TP_TABLE_SIZE].searchedDepth = depth;
      tpTable[currentZobristKey%TP_TABLE_SIZE].searchedScore = currentScore;
    }


    if(alpha < currentScore)
    {
      alpha = currentScore;
      if(beta <= currentScore)
      {
        killerMove[depth][1] = killerMove[depth][0];
        killerMove[depth][0] = newPositions[i].lastMove;
        break;
      }
    }
  }
  //check for MATE or STALEMATE
  if(numberLegalMoves == 0)
  {
    if(isKingInCheck(us, enemy, origPosition))
    {
      alpha = -SCORE_MATE-depth;
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
  initTranspositionTable(tpTable);
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
  int numberLegalMoves = 0;
  Score alpha = -SCORE_INFINITY;
  Score beta = SCORE_INFINITY;
  Score currentScore;
  PositionList newPositions;
  int bestBoardIndex = 0;
  generateAllMoves(us, enemy, origPosition, newPositions);
  sortMoves(us, enemy, origPosition, newPositions, killerMove[depth]);
  for(int i = 0; i < newPositions.size; i++)
  {
    if(isKingInCheck(us, enemy, newPositions[i]))
    {
      continue;
    }
    numberLegalMoves++;
    currentScore = -negaMax(enemy, us, newPositions[i], depth - 1, -beta, -alpha);
    if(alpha < currentScore)
    {
      alpha = currentScore;
      bestBoardIndex = i;
    }
    //printPosition(newPositions[i]);
    //std::cout << "Score: " << currentScore << std::endl;
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  if(numberLegalMoves == 0)
  {
    std::cout << std::endl << std::endl << "NO LEGAL MOVES" << std::endl << std::endl;
  }
  else
  {
    printPosition(newPositions[bestBoardIndex]);
  }
  newPositions[bestBoardIndex].lastMove.print();
  std::cout << "Score: ";
  if(alpha >= SCORE_MATE)
  {
    std::cout << "Mating in " << (depth - (alpha - SCORE_MATE))/2 << std::endl;
  }
  else if (alpha <= -SCORE_MATE)
  {
    std::cout << "Mated in " << (depth - (-alpha - SCORE_MATE))/2 << std::endl;
  }
  else
  {
    std::cout << alpha << std::endl;
  }
  std::cout << "Time elapsed: " << timeElapsed << std::endl;
  std::cout << "Nodes: " << nodes << std::endl;
  std::cout << "Nodes/Second: " << (int)(nodes / (0.001 * timeElapsed)) << std::endl;

  return newPositions[bestBoardIndex];
}
