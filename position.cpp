#include "position.hpp"
#include "bitMasks.hpp"
using namespace BitMasks;
//0b 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
void Position::setToStartposition()
{
  whitePieces = 0b0000000000000000000000000000000000000000000000001111111111111111;
  blackPieces = 0b1111111111111111000000000000000000000000000000000000000000000000;
  pawns = 0b0000000011111111000000000000000000000000000000001111111100000000;
  knights = 0b0100001000000000000000000000000000000000000000000000000001000010;
  bishops = 0b0010010000000000000000000000000000000000000000000000000000100100;
  rooks = 0b1000000100000000000000000000000000000000000000000000000010000001;
  queens = 0b0000100000000000000000000000000000000000000000000000000000001000;
  kings = 0b0001000000000000000000000000000000000000000000000000000000010000;
  properties = 0b0 | bitAtIndex[20];
}


int Position::getNumberHalfmovesPlayed()
{
  return properties & 0b1111111111;
}
int Position::getNumberMovesPlayed()
{
  return (properties & 0b1111111111) / 2;
}
bool Position::whiteToMove()
{
  return properties & bitAtIndex[20];
}
bool Position::blackToMove()
{
  return !((bool)(properties & bitAtIndex[20]));
}
