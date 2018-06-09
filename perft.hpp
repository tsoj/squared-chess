#pragma once

#include "position.hpp"
#include "debug.hpp"

namespace Perft
{
  uint64_t perft(int depth, const Position& origPosition)
  {
    if(depth == 0)
    {
      return 1;
    }

    uint64_t nodes = 0;
    MoveList moveList = origPosition.generateMoveList();
    for(size_t i = 0; i<moveList.size(); i++)
    {
      Position newPosition = origPosition;
      newPosition.makeMove(moveList[i]);
      ASSERT(newPosition.calculateZobristKey() == newPosition.zobristKey);
      if(not newPosition.inCheck(origPosition.us, origPosition.enemy))
      {
        nodes += perft(depth - 1, newPosition);
      }
    }
    return nodes;
  }

  void test()
  {
    Position p;
    p.setFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    ASSERT(perft(4, p) == 4'085'603);
    p.setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    ASSERT(perft(5, p) == 4'865'609);
    p.setFromFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    ASSERT(perft(4, p) == 3'894'594);
    p.setFromFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 ");
    ASSERT(perft(4, p) == 2'103'487);
    p.setFromFen("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1");
    ASSERT(perft(5, p) == 15'833'292);
    p.setFromFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    ASSERT(perft(6, p) == 11'030'083);
  }
}
