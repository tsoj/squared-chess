#include <iostream>
#include <vector>
#include <sys/resource.h>
#include "bitOperations.hpp"
#include "position.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "bitMasks.hpp"
#include "text.hpp"
#include "movegen.hpp"
#include "perft.hpp"
#include "evaluation.hpp"
#include "search.hpp"

void initStack()
{
  const rlim_t stackSize = 0x2000000;
  struct rlimit r1;
  int result;
  result = getrlimit(RLIMIT_STACK, &r1);
  if(result == 0)
  {
    if(r1.rlim_cur < stackSize)
    {
      r1.rlim_cur = stackSize;
      result = setrlimit(RLIMIT_STACK, &r1);
    }
  }
}

using namespace BitMasks;
int main()
{
  //initStack();
  Position p;
  p = setPositionFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  printPosition(p);
  //printPositionBitboards(p);
  std::cout << std::endl <<  negaMax(WHITE, BLACK, p, 6) << std::endl;
  /*std::vector<Position> newPositions;
  Bitboard occupancy = p.colors[WHITE] | p.colors[BLACK];
  generateAllMoves(WHITE, BLACK, p, newPositions);
  for(int i = 0; i<newPositions.size(); i++)
  {
    printPosition(newPositions[i]);
    //printPositionBitboards(newPositions[i]);
  }*/
}
