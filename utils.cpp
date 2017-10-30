#include <iostream>
#include <array>
#include "utils.hpp"
#include "bitMasks.hpp"
#include <string>
using namespace BitMasks;

#define horizontalLineSymbol "\u2501" //http://www.fileformat.info/info/unicode/category/So/list.htm
#define verticalLineSymbol "\u2503"
#define ONE "\u23FA"
#define ZERO " "
#define BLACK_PAWN_UNICODE "\u2659"
#define BLACK_KNIGHT_UNICODE "\u2658"
#define BLACK_BISHOP_UNICODE "\u2657"
#define BLACK_ROOK_UNICODE "\u2656"
#define BLACK_QUEEN_UNICODE "\u2655"
#define BLACK_KING_UNICODE "\u2654"
#define WHITE_PAWN_UNICODE "\u265F"
#define WHITE_KNIGHT_UNICODE "\u265E"
#define WHITE_BISHOP_UNICODE "\u265D"
#define WHITE_ROOK_UNICODE "\u265C"
#define WHITE_QUEEN_UNICODE "\u265B"
#define WHITE_KING_UNICODE "\u265A"

void print64FieldsToChessboard(std::array<std::string, 64> fieldContent)
{

  std::cout << std::endl;for(int k = 0; k < 33; k++)
  {
    std::cout << horizontalLineSymbol;
  }
  std::cout << std::endl;
  	for (int i = 7; i >= 0; i--)
    {
      for(int j = 0; j < 8; j++)
      {
        std::cout << verticalLineSymbol << ' ' << fieldContent[8*i + j] << ' ';
      }
    	std::cout << verticalLineSymbol << " " << i+1;
      std::cout << std::endl;
      for(int k = 0; k < 33; k++)
      {
        std::cout << horizontalLineSymbol;
      }
      std::cout << std::endl;

  	}
  	std::cout << "  A   B   C   D   E   F   G   H" << std::endl;
}
void printBitboard(Bitboard bitboard)
{
  std::array<std::string, 64> temp;
  for(int i = 0; i<64; i++)
  {

    temp[i] = ZERO;
    if(bitboard & bitAtIndex[i])
    {
      temp[i] = ONE;
    }
  }
  print64FieldsToChessboard(temp);
}
void printPosition(Position position)
{
    std::array<std::string, 64> temp;
    for(int i = 0; i<64; i++)
    {
      temp[i] = " ";
      if(position.blackPieces & bitAtIndex[i])
      {
        if(position.pawns & bitAtIndex[i])
        {
          temp[i] = BLACK_PAWN_UNICODE;
        }
        else if(position.knights & bitAtIndex[i])
        {
          temp[i] = BLACK_KNIGHT_UNICODE;
        }
        else if(position.bishops & bitAtIndex[i])
        {
          temp[i] = BLACK_BISHOP_UNICODE;
        }
        else if(position.rooks & bitAtIndex[i])
        {
          temp[i] = BLACK_ROOK_UNICODE;
        }
        else if(position.queens & bitAtIndex[i])
        {
          temp[i] = BLACK_QUEEN_UNICODE;
        }
        else if(position.kings & bitAtIndex[i])
        {
          temp[i] = BLACK_KING_UNICODE;
        }
      }
      else if(position.whitePieces & bitAtIndex[i])
      {
        if(position.pawns & bitAtIndex[i])
        {
          temp[i] = WHITE_PAWN_UNICODE;
        }
        else if(position.knights & bitAtIndex[i])
        {
          temp[i] = WHITE_KNIGHT_UNICODE;
        }
        else if(position.bishops & bitAtIndex[i])
        {
          temp[i] = WHITE_BISHOP_UNICODE;
        }
        else if(position.rooks & bitAtIndex[i])
        {
          temp[i] = WHITE_ROOK_UNICODE;
        }
        else if(position.queens & bitAtIndex[i])
        {
          temp[i] = WHITE_QUEEN_UNICODE;
        }
        else if(position.kings & bitAtIndex[i])
        {
          temp[i] = WHITE_KING_UNICODE;
        }
      }
    }
    print64FieldsToChessboard(temp);
    std::cout << position.getNumberMovesPlayed() <<" moves played." << std::endl;
    if(position.whiteToMove())
    {
      std::cout << "White to move." << std::endl;
    }
    else
    {
      std::cout << "Black to move." << std::endl;
    }
}
