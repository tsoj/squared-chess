#include <iostream>
#include <vector>
#include "bitOperations.hpp"
#include "position.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "bitMasks.hpp"
#include "text.hpp"
#include "movegen.hpp"
#include "perft.hpp"

using namespace BitMasks;
int main()
{
  Position p;
  p = setPositionFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
  printPosition(p);
  //printPositionBitboards(p);
  std::cout << perft(2, p) << std::endl;
  /*std::vector<Position> newPositions;
  Bitboard occupancy = p.colors[WHITE] | p.colors[BLACK];
  generateAllMoves(WHITE, BLACK, p, newPositions);
  for(int i = 0; i<newPositions.size(); i++)
  {
    printPosition(newPositions[i]);
    //printPositionBitboards(newPositions[i]);
  }*/
}
