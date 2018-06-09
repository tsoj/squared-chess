#include "position.hpp"
#include "chessData.hpp"
#include "countZeros.hpp"
#include "move.hpp"
#include "moveList.hpp"

#include <string>
#include <sstream>

using namespace ChessData;
using namespace CountZeros;

Position Position::emptyPosition()
{
  return Position
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
void Position::addPiece(const Player player, const Piece piece, const uint64_t to)
{
  this->pieces[piece] |= to;
  this->players[player] |= to;
}
void Position::removePiece(const Player player, const Piece piece, const uint64_t from)
{
  this->pieces[piece] &=  ~from;
  this->players[player] &=  ~from;
}
void Position::movePiece(const Player player, const Piece piece, const uint64_t from, const uint64_t to)
{
  this->removePiece(player, piece, from);
  this->addPiece(player, piece, to);
}
std::string Position::getString() const
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
std::string Position::getDataString() const
{
  std::string ret = "";
  ret += "\nWHOSE MOVE: ";
  ret += std::to_string(us);
  ret += "\nFULLMOVES PLAYED: ";
  ret += std::to_string(fullmovesPlayed);
  ret += "\nHALFMOVE CLOCK: ";
  ret += std::to_string(halfmoveClock);
  ret += "\nZOBRIST KEY: ";
  ret += std::to_string(zobristKey);
  ret += "\nCASTLING / EN PASSANT\n";
  ret += getBitboardString(enPassantCastling);
  ret += "WHITE:\n";
  ret += getBitboardString(players[WHITE]);
  ret += "BLACK:\n";
  ret += getBitboardString(players[BLACK]);
  ret += "PAWNS:\n";
  ret += getBitboardString(pieces[PAWN]);
  ret += "KNIGHTS:\n";
  ret += getBitboardString(pieces[KNIGHT]);
  ret += "BISHOPS:\n";
  ret += getBitboardString(pieces[BISHOP]);
  ret += "ROOKS:\n";
  ret += getBitboardString(pieces[ROOK]);
  ret += "QUEENS:\n";
  ret += getBitboardString(pieces[QUEEN]);
  ret += "KINGS:\n";
  ret += getBitboardString(pieces[KING]);
  return ret;
}
uint64_t Position::calculateZobristKey() const
{
  uint64_t ret = 0;
  for(Piece i = 0; i<NO_PIECE; i++)
  {
    if(pieces[i]!=0)
    {
      uint64_t tmpOccupancy = pieces[i];
      while(tmpOccupancy != 0)
      {
        size_t squareIndex = CountZeros::findAndClearTrailingOne(tmpOccupancy);
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
uint64_t Position::getUpdatedZobristKey(const Move& m) const
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
    const size_t capturedIndex = CountZeros::trailingZeros(PAWN_QUIET_ATTACK_TABLE[this->enemy][m.to]);
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
bool Position::setFromFen(const std::string fen)
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
  this->zobristKey = calculateZobristKey();
  return true;
}
bool Position::inCheck(Player us, Player enemy, size_t kingsIndex) const
{
  const uint64_t occupancy = this->players[WHITE] | this->players[BLACK];
  // QUEEN
  if((getAttackMask<QUEEN>(kingsIndex, occupancy) & this->pieces[QUEEN] & this->players[enemy]) != 0)
  {
    return true;
  }
  // KNIGHT
  if((getAttackMask<KNIGHT>(kingsIndex, occupancy) & this->pieces[KNIGHT] & this->players[enemy]) != 0)
  {
    return true;
  }
  // BISHOP
  if((getAttackMask<BISHOP>(kingsIndex, occupancy) & this->pieces[BISHOP] & this->players[enemy]) != 0)
  {
    return true;
  }
  // ROOK
  if((getAttackMask<ROOK>(kingsIndex, occupancy) & this->pieces[ROOK] & this->players[enemy]) != 0)
  {
    return true;
  }
  // KING
  if((getAttackMask<KING>(kingsIndex, occupancy) & this->pieces[KING] & this->players[enemy]) != 0)
  {
    return true;
  }
  //PAWN
  if((PAWN_CAPTURE_ATTACK_TABLE[us][kingsIndex] & this->pieces[PAWN] & this->players[enemy]) != 0)
  {
    return true;
  }
  return false;
}
bool Position::inCheck(Player us, Player enemy) const
{
    const size_t kingsIndex = trailingZeros(this->pieces[KING] & this->players[us]);
    ASSERT(kingsIndex<64);
    return this->inCheck(us, enemy, kingsIndex);
}
MoveList Position::generateMoveList() const
{
  MoveList moveList = MoveList();
  const uint64_t newEnPassantCastling = this->enPassantCastling & (RANKS[0] | RANKS[7]);
  moveList.generateCastlingMoves((*this), newEnPassantCastling);
  moveList.generateMoves<PAWN>((*this), newEnPassantCastling, false);
  moveList.generateMoves<KNIGHT>((*this), newEnPassantCastling, false);
  moveList.generateMoves<BISHOP>((*this), newEnPassantCastling, false);
  moveList.generateMoves<ROOK>((*this), newEnPassantCastling, false);
  moveList.generateMoves<QUEEN>((*this), newEnPassantCastling, false);
  moveList.generateMoves<KING>((*this), newEnPassantCastling, false);
  return moveList;
}
MoveList Position::generateCaptureMoveList() const
{
  MoveList moveList = MoveList();
  const uint64_t newEnPassantCastling = this->enPassantCastling & (RANKS[0] | RANKS[7]);
  moveList.generateMoves<PAWN>((*this), newEnPassantCastling, true);
  moveList.generateMoves<KNIGHT>((*this), newEnPassantCastling, true);
  moveList.generateMoves<BISHOP>((*this), newEnPassantCastling, true);
  moveList.generateMoves<ROOK>((*this), newEnPassantCastling, true);
  moveList.generateMoves<QUEEN>((*this), newEnPassantCastling, true);
  moveList.generateMoves<KING>((*this), newEnPassantCastling, true);
  return moveList;
}
void Position::makeMove(Move m)
{
  const Player enemy = this->enemy;
  const Player us = this->us;
  this->enPassantCastling = m.enPassantCastling;
  // en passant
  if(m.capturedEnPassant)
  {
    this->removePiece(enemy, PAWN, PAWN_QUIET_ATTACK_TABLE[enemy][m.to]);
    this->movePiece(us, PAWN, BIT_AT_INDEX[m.from], BIT_AT_INDEX[m.to]);
  }
  // castling
  else if(m.castled)
  {
    // QUEENSIDE
    if(m.to == CASTLING_QUEENSIDE_KING_TO_INDEX[us])
    {
      this->movePiece(us, KING, CASTLING_KING_FROM[us], CASTLING_QUEENSIDE_KING_TO[us]);
      this->movePiece(us, ROOK, CASTLING_QUEENSIDE_ROOK_FROM[us], CASTLING_QUEENSIDE_ROOK_TO[us]);
    }
    // KINGSIDE
    else
    {
      this->movePiece(us, KING, CASTLING_KING_FROM[us], CASTLING_KINGSIDE_KING_TO[us]);
      this->movePiece(us, ROOK, CASTLING_KINGSIDE_ROOK_FROM[us], CASTLING_KINGSIDE_ROOK_TO[us]);
    }
  }
  else
  {
    if(m.captured != NO_PIECE)
    {
      this->removePiece(enemy, m.captured, BIT_AT_INDEX[m.to]);
    }
    if(m.promoted == NO_PIECE)
    {
      this->movePiece(us, m.moved, BIT_AT_INDEX[m.from], BIT_AT_INDEX[m.to]);
    }
    else
    {
      this->removePiece(us, m.moved, BIT_AT_INDEX[m.from]);
      this->addPiece(us, m.promoted, BIT_AT_INDEX[m.to]);
    }
  }
  if(m.moved == PAWN || m.captured != NO_PIECE)
  {
    this->halfmoveClock = 0;
  }
  else
  {
    this->halfmoveClock += 1;
  }
  if(this->us == BLACK)
  {
    this->fullmovesPlayed += 1;
  }
  this->us = enemy;
  this->enemy = us;
  this->zobristKey = m.zobristKey;
}
std::string Position::getAllPseudoLegalMovesString() const
{
  std::string ret = "";
  const MoveList moveList = this->generateMoveList();
  for(size_t i = 0; i!=moveList.size(); ++i)
  {
    Position p = (*this);
    p.makeMove(moveList[i]);
    ret += "------------------------------------------------\n";
    ret += moveList[i].getNotation()+":\n";
    ret += p.getString();
    ret += "\n";
    //ret += p.getDataString();
  }
  ret += std::to_string(moveList.size());
  ret += " pseudo-legal moves.\n";
  return ret;
}
