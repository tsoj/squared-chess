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

using namespace BitMasks;
int main()
{
  //initStack();
  Position p;
  p = setPositionFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  printPosition(p);
  //printPositionBitboards(p);
  //std::cout << std::endl <<  negaMax(WHITE, BLACK, p, 5, -POSITION_VALUE_INFINITY, POSITION_VALUE_INFINITY) << std::endl;
  p = startSearchMinMax(p, 6);
  p = startSearchMinMax(p, 5);
  p = startSearchMinMax(p, 4);
  p = startSearchMinMax(p, 3);
  p = startSearchMinMax(p, 2);
  p = startSearchMinMax(p, 1);
  /*std::vector<Position> newPositions;
  Bitboard occupancy = p.colors[WHITE] | p.colors[BLACK];
  generateAllMoves(WHITE, BLACK, p, newPositions);
  for(int i = 0; i<newPositions.size(); i++)
  {
    printPosition(newPositions[i]);
    //printPositionBitboards(newPositions[i]);
  }*/
}
