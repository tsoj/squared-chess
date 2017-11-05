#include <iostream>
#include <array>
#include "utils.hpp"
#include "bitMasks.hpp"
#include <string>
#include <sstream>
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
      if(position.colors[BLACK] & bitAtIndex[i])
      {
        if(position.pieces[PAWN] & bitAtIndex[i])
        {
          temp[i] = BLACK_PAWN_UNICODE;
        }
        else if(position.pieces[KNIGHT] & bitAtIndex[i])
        {
          temp[i] = BLACK_KNIGHT_UNICODE;
        }
        else if(position.pieces[BISHOP] & bitAtIndex[i])
        {
          temp[i] = BLACK_BISHOP_UNICODE;
        }
        else if(position.pieces[ROOK] & bitAtIndex[i])
        {
          temp[i] = BLACK_ROOK_UNICODE;
        }
        else if(position.pieces[QUEEN] & bitAtIndex[i])
        {
          temp[i] = BLACK_QUEEN_UNICODE;
        }
        else if(position.pieces[KING] & bitAtIndex[i])
        {
          temp[i] = BLACK_KING_UNICODE;
        }
      }
      else if(position.colors[WHITE] & bitAtIndex[i])
      {
        if(position.pieces[PAWN] & bitAtIndex[i])
        {
          temp[i] = WHITE_PAWN_UNICODE;
        }
        else if(position.pieces[KNIGHT] & bitAtIndex[i])
        {
          temp[i] = WHITE_KNIGHT_UNICODE;
        }
        else if(position.pieces[BISHOP] & bitAtIndex[i])
        {
          temp[i] = WHITE_BISHOP_UNICODE;
        }
        else if(position.pieces[ROOK] & bitAtIndex[i])
        {
          temp[i] = WHITE_ROOK_UNICODE;
        }
        else if(position.pieces[QUEEN] & bitAtIndex[i])
        {
          temp[i] = WHITE_QUEEN_UNICODE;
        }
        else if(position.pieces[KING] & bitAtIndex[i])
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
Position setPositionFromFen(std::string fenString)
{
  Position position;
  position.clearBoard();

  std::string rank[8];
  std::string whoIsToMove;
  std::string castling;
  std::string enPassant;
  std::string numberMoves;
  std::string fiftyMoveRuleCouter;

	std::stringstream fenStream(fenString);
  for(int i = 7; i > 0; i--)
  {
    getline(fenStream, rank[i], '/');
  }
  fenStream >> rank[0];
  fenStream >> whoIsToMove;
  fenStream >> castling;
  fenStream >> enPassant;
  fenStream >> numberMoves;
  fenStream >> fiftyMoveRuleCouter;

  for(int i = 0; i < 8; i++)
  {
    static int fieldIndex = 0;
    for(int j = 0; j<rank[i].size(); j++)
    {
      if(rank[i][j] == '1')
      {
        fieldIndex+=1;
      }
      else if(rank[i][j] == '2')
      {
        fieldIndex+=2;
      }
      else if(rank[i][j] == '3')
      {
        fieldIndex+=3;
      }
      else if(rank[i][j] == '4')
      {
        fieldIndex+=4;
      }
      else if(rank[i][j] == '5')
      {
        fieldIndex+=5;
      }
      else if(rank[i][j] == '6')
      {
        fieldIndex+=6;
      }
      else if(rank[i][j] == '7')
      {
        fieldIndex+=7;
      }
      else if(rank[i][j] == '8')
      {
        fieldIndex+=8;
      }
      else if(rank[i][j] == 'p')
      {
        position.pieces[PAWN] |= bitAtIndex[fieldIndex];
        position.colors[BLACK] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'n')
      {
        position.pieces[KNIGHT] |= bitAtIndex[fieldIndex];
        position.colors[BLACK] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'b')
      {
        position.pieces[BISHOP] |= bitAtIndex[fieldIndex];
        position.colors[BLACK] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'r')
      {
        position.pieces[ROOK] |= bitAtIndex[fieldIndex];
        position.colors[BLACK] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'q')
      {
        position.pieces[QUEEN] |= bitAtIndex[fieldIndex];
        position.colors[BLACK] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'k')
      {
        position.pieces[KING] |= bitAtIndex[fieldIndex];
        position.colors[BLACK] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'P')
      {
        position.pieces[PAWN] |= bitAtIndex[fieldIndex];
        position.colors[WHITE] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'N')
      {
        position.pieces[KNIGHT] |= bitAtIndex[fieldIndex];
        position.colors[WHITE] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'B')
      {
        position.pieces[BISHOP] |= bitAtIndex[fieldIndex];
        position.colors[WHITE] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'R')
      {
        position.pieces[ROOK] |= bitAtIndex[fieldIndex];
        position.colors[WHITE] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'Q')
      {
        position.pieces[QUEEN] |= bitAtIndex[fieldIndex];
        position.colors[WHITE] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
      else if(rank[i][j] == 'K')
      {
        position.pieces[KING] |= bitAtIndex[fieldIndex];
        position.colors[WHITE] |= bitAtIndex[fieldIndex];
        fieldIndex+=1;
      }
    }
  }
  if(whoIsToMove == "w")
  {
    position.whoIsToMove = WHITE_TO_MOVE;
  }
  else
  {
    position.whoIsToMove = BLACK_TO_MOVE;
  }
  for(int i = 0; i<castling.size(); i++)
  {
    if(castling[i] == 'K')
    {
      position.castling[WHITE] |= bitAtIndex[castlingRookKingsideFrom[WHITE]] | bitAtIndex[castlingKingFrom[WHITE]];

    }
    else if(castling[i] == 'Q')
    {
      position.castling[WHITE] |= bitAtIndex[castlingRookQueensideFrom[WHITE]] | bitAtIndex[castlingKingFrom[WHITE]];
    }
    else if(castling[i] == 'k')
    {
      position.castling[BLACK] |= bitAtIndex[castlingRookKingsideFrom[BLACK]] | bitAtIndex[castlingKingFrom[BLACK]];
    }
    else if(castling[i] == 'q')
    {
      position.castling[BLACK] |= bitAtIndex[castlingRookQueensideFrom[BLACK]] | bitAtIndex[castlingKingFrom[BLACK]];
    }
  }
  int row = -1;
  int file = -1;
  for(int i = 0; i<enPassant.size(); i++)
  {
    if(enPassant[i] == '3')
    {
      row = 2;
    }
    else if(enPassant[i] == '6')
    {
      row = 5;
    }
    else if(enPassant[i] == 'a')
    {
      file = 0;
    }
    else if(enPassant[i] == 'b')
    {
      file = 1;
    }
    else if(enPassant[i] == 'c')
    {
      file = 2;
    }
    else if(enPassant[i] == 'd')
    {
      file = 3;
    }
    else if(enPassant[i] == 'e')
    {
      file = 4;
    }
    else if(enPassant[i] == 'f')
    {
      file = 5;
    }
    else if(enPassant[i] == 'g')
    {
      file = 6;
    }
    else if(enPassant[i] == 'h')
    {
      file = 7;
    }
  }
  if(row != -1 && file != -1)
  {
    position.enPassant = bitAtIndex[row*8 + file];
  }
  if(numberMoves != "")
  {
    position.numberHalfmovesPlayed = 2* stoi(numberMoves);
    if(whoIsToMove == "b")
    {
      position.numberHalfmovesPlayed += 1;
    }
  }
  return position;
}

void printPositionBitboards(Position position)
{
  std::cout << "White pieces:" << std::endl;
  printBitboard(position.colors[WHITE]);
  std::cout << "Black pieces:" << std::endl;
  printBitboard(position.colors[BLACK]);
  std::cout << "Pawns:" << std::endl;
  printBitboard(position.pieces[PAWN]);
  std::cout << "Knights:" << std::endl;
  printBitboard(position.pieces[KNIGHT]);
  std::cout << "Bishops:" << std::endl;
  printBitboard(position.pieces[BISHOP]);
  std::cout << "Rooks:" << std::endl;
  printBitboard(position.pieces[ROOK]);
  std::cout << "Queens:" << std::endl;
  printBitboard(position.pieces[QUEEN]);
  std::cout << "Kings:" << std::endl;
  printBitboard(position.pieces[KING]);
  std::cout << "enPassant:" << std::endl;
  printBitboard(position.enPassant);
  std::cout << "castling white:" << std::endl;
  printBitboard(position.castling[WHITE]);
  std::cout << "castling black:" << std::endl;
  printBitboard(position.castling[BLACK]);
  std::cout << "last piece moved to index:" << std::endl;
  std::cout << position.lastPieceMovedToIndex << std::endl;
  std::cout << "last piece moved from index:" << std::endl;
  std::cout << position.lastPieceMovedFromIndex << std::endl;
  std::cout << "last piecetype moved:" << std::endl;
  std::cout << position.lastMovedPieceType << std::endl;
  std::cout << "last piecestype captured:" << std::endl;
  std::cout << position.lastCapturedPieceType << std::endl;
}
