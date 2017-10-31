#include <iostream>
#include "bitOperations.hpp"
#include "position.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "bitMasks.hpp"
#include "text.hpp"

using namespace BitMasks;
int main()
{
  //std::cout << LOGO << std::endl;
  printBitboard(possibleRanks[11]);
  printBitboard(getAttackBitboard<QUEEN>(possibleRanks[11], 20));
  return 0;
}
