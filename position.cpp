#include "position.hpp"
#include "chessData.hpp"
#include "move.hpp"

#include <string>
#include <sstream>

using namespace ChessData;

Position Position::emptyPosition()
{
  return
  Position
  {
    {0,0,0,0,0,0},
    {0,0},
    0,
    0,
    ChessTypes::NO_PLAYER,
    ChessTypes::NO_PLAYER,
    0,
    0
  };
}
void Position::addPiece(Player player, Piece piece, uint64_t to)
{
  this->pieces[piece] |= to;
  this->players[player] |= to;
}
void Position::removePiece(Player player, Piece piece, uint64_t from)
{
  this->pieces[piece] &=  !from;
  this->players[player] &=  !from;
}
void Position::movePiece(Player player, Piece piece, uint64_t from, uint64_t to)
{
  this->removePiece(player, piece, from);
  this->addPiece(player, piece, to);
}
std::string Position::getString()
{
  std::string ret = "\n _ _ _ _ _ _ _ _\n";
  for(size_t rank = 7; rank<8; rank--)
  {
    for(size_t file = 0; file<8; file++)
    {
      bool foundPiece = false;
      for(Player player = 0; player<NO_PLAYER; player++)
      {
        for(Piece piece = 0; piece<NO_PIECE; piece++)
        {
          if((BIT_AT_INDEX[8*rank + file] & this->pieces[piece] & this->players[player]) != 0)
          {
            ret += "|\u0332"+getUnicodePiece(player, piece);
            foundPiece = true;
          }
        }
      }
      if(foundPiece == false)
      {
        ret += "|_";
      }
    }
    ret+="|\n";
  }
  return ret;
}
uint64_t Position::calculateZobristkey()
{
  uint64_t ret = 0;
  for(Piece i = 0; i<NO_PIECE; i++)
  {
    if(pieces[i]!=0)
    {
      uint64_t tmpOccupancy = pieces[i];
      while(tmpOccupancy != 0)
      {
        size_t squareIndex = ChessData::findAndClearTrailingOne(tmpOccupancy);
        if((BIT_AT_INDEX[squareIndex] & players[WHITE]) != 0)
        {
          ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[WHITE][squareIndex];
        }
        else
        {
          ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[BLACK][squareIndex];
        }
        ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[i][squareIndex];
      }
    }
  }
  ret ^= enPassantCastling;
  ret ^= (uint64_t)us;
  return ret;
}
uint64_t Position::getUpdatedZobristkey(const Move& m)
{
  uint64_t ret = this->zobristKey;
  ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.moved][m.from];
  if(m.promoted != NO_PIECE)
  {
    ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.promoted][m.to];
  }
  else
  {
    ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.moved][m.to];
  }

  ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->us][m.from];
  ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->us][m.to];

  if(m.captured != NO_PIECE && !m.capturedEnPassant)
  {
    ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.captured][m.to];
    ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->enemy][m.to];
  }

  ret ^= this->enPassantCastling;
  ret ^= m.enPassantCastling;
  ret ^= (uint64_t)WHITE;
  ret ^= (uint64_t)BLACK;

  if(m.capturedEnPassant)
  {
    size_t capturedIndex = CountZeros::trailingZeros(PAWN_QUIET_ATTACK_TABLE[this->enemy][m.to]);
    ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[PAWN][capturedIndex];
    ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->enemy][capturedIndex];
  }

  if(m.castled)
  {
    //IF QUEENSIDE
    if(m.to == CASTLING_QUEENSIDE_KING_TO_INDEX[this->us])
    {
      ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_QUEENSIDE_ROOK_FROM_INDEX[this->us]];
      ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_QUEENSIDE_ROOK_TO_INDEX[this->us]];
      ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->us][CASTLING_QUEENSIDE_ROOK_FROM_INDEX[this->us]];
      ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->us][CASTLING_QUEENSIDE_ROOK_TO_INDEX[this->us]];
    }
    //IF KINGSIDE
    else
    {
      ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_KINGSIDE_ROOK_FROM_INDEX[this->us]];
      ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_KINGSIDE_ROOK_TO_INDEX[this->us]];
      ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->us][CASTLING_KINGSIDE_ROOK_FROM_INDEX[this->us]];
      ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[this->us][CASTLING_KINGSIDE_ROOK_TO_INDEX[this->us]];
    }
  }
  return ret;
}
bool Position::setFromFen(std::string fen)
{
  *this = emptyPosition();
  std::stringstream buffer = std::stringstream(fen);
  std::string pieces;
  std::getline(buffer, pieces, ' ');
  std::string activeColor;
  std::getline(buffer, activeColor, ' ');
  std::string castlingAvailability;
  std::getline(buffer, castlingAvailability, ' ');
  std::string enPassantTargetSquare;
  std::getline(buffer, enPassantTargetSquare, ' ');
  std::string halfmoveClock;
  std::getline(buffer, halfmoveClock, ' ');
  std::string fullmoveNumber;
  std::getline(buffer, fullmoveNumber, ' ');

  size_t squareCounter = 56;
  for(auto i : pieces)
  {
    switch(i)
    {
      case '/':
      {
        squareCounter-=16;
        break;
      }
      case '8':
      {
        squareCounter+=8;
        break;
      }
      case '7':
      {
        squareCounter+=7;
        break;
      }
      case '6':
      {
        squareCounter+=6;
        break;
      }
      case '5':
      {
        squareCounter+=5;
        break;
      }
      case '4':
      {
        squareCounter+=4;
        break;
      }
      case '3':
      {
        squareCounter+=3;
        break;
      }
      case '2':
      {
        squareCounter+=2;
        break;
      }
      case '1':
      {
        squareCounter+=1;
        break;
      }
      case '0':
      {
        squareCounter+=0;
        break;
      }
      case 'P':
      {
        this->addPiece(WHITE, PAWN, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'N':
      {
        this->addPiece(WHITE, KNIGHT, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'B':
      {
        this->addPiece(WHITE, BISHOP, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'R':
      {
        this->addPiece(WHITE, ROOK, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'Q':
      {
        this->addPiece(WHITE, QUEEN, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'K':
      {
        this->addPiece(WHITE, KING, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'p':
      {
        this->addPiece(BLACK, PAWN, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'n':
      {
        this->addPiece(BLACK, KNIGHT, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'b':
      {
        this->addPiece(BLACK, BISHOP, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'r':
      {
        this->addPiece(BLACK, ROOK, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'q':
      {
        this->addPiece(BLACK, QUEEN, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      case 'k':
      {
        this->addPiece(BLACK, KING, BIT_AT_INDEX[squareCounter]);
        squareCounter+=1;
        break;
      };
      default: return false;
    }
  }
  if(activeColor == "w" || activeColor == "W")
  {
    this->us = WHITE;
    this->enemy = BLACK;
  }
  else if(activeColor == "b" || activeColor == "B")
  {
    this->us = BLACK;
    this->enemy = WHITE;
  }
  else
  {
    return false;
  }
  for(auto i : castlingAvailability)
  {
    switch(i)
    {
      case '-':
      {
        break;
      }
      case 'K':
      {
        this->enPassantCastling |= CASTLING_KINGSIDE_ROOK_FROM[WHITE] | CASTLING_KING_FROM[WHITE];
        break;
      }
      case 'k':
      {
        this->enPassantCastling |= CASTLING_KINGSIDE_ROOK_FROM[BLACK] | CASTLING_KING_FROM[BLACK];
        break;
      }
      case 'Q':
      {
        this->enPassantCastling |= CASTLING_QUEENSIDE_ROOK_FROM[WHITE] | CASTLING_KING_FROM[WHITE];
        break;
      }
      case 'q':
      {
        this->enPassantCastling |= CASTLING_QUEENSIDE_ROOK_FROM[BLACK] | CASTLING_KING_FROM[BLACK];
        break;
      }
      default: return false;
    }
  }
  if(enPassantTargetSquare != "-")
  {
    size_t enPassantTargetSquareIndex = getSquareIndex(enPassantTargetSquare);
    if(enPassantTargetSquareIndex>=64)
    {
      return false;
    }
    this->enPassantCastling |= BIT_AT_INDEX[enPassantTargetSquareIndex];
  }
  this->halfmoveClock = std::stoi(halfmoveClock);
  this->fullmovesPlayed = std::stoi(fullmoveNumber);
  zobristKey = calculateZobristkey();
  return true;
}
