#include <iostream>
#include <vector>
#include <sys/resource.h>
#include "bitOperations.hpp"
#include "position.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "data.hpp"
#include "text.hpp"
#include "movegen.hpp"
#include "evaluation.hpp"
#include "search.hpp"
#include "moveOrdering.hpp"

using namespace Data;
int main()
{
  //initStack();
  std::string input;
  getline(std::cin, input);
  Position p;
  p = setPositionFromFen(input);
  //p = setPositionFromFen("5r1k/1p1b1p1p/p2ppb2/5P1B/1q6/1Pr3R1/2PQ2PP/5R1K w - - 0 1");
  //p = setPositionFromFen("1k6/8/K7/8/8/8/Q7/8 w - -");
  //p = setPositionFromFen("r3k2r/pp2n2p/n1p3p1/2P2p2/2bNB3/P4P2/1P4PP/RN2K2R w KQkq -");
  //p = setPositionFromFen("k7/2Q5/1K6/8/8/8/8/8 w - - 0 0");
  printPosition(p);
  //printPositionBitboards(p);
  if(false)
  {
    PositionList newPositions;
    newPositions.size = 0;
    generateAllMoves(WHITE, BLACK, p, newPositions);
    for(int i = 0; i<newPositions.size; i++)
    {
      printPosition(newPositions[i]);
      newPositions[i].lastMove.print();
      printPositionBitboards(newPositions[i]);
    }
  }
  while(true)
  {
    getline(std::cin, input);
    p = startSearch(p, 8);
  }
  if(false)
  {
    Move m1;
    Move m2;
    m2.from=13;
    m1.from=13;
    m2.to=40;
    m1.to=40;
    if(m1==m2)
    {
      std::cout <<"hi"<<std::endl;
    }
  }
}
