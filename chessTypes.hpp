#pragma once

#include <string>
#include <tuple>

namespace ChessTypes
{
  typedef int32_t Score;

  typedef size_t Player;

  const Player WHITE = 0;
  const Player BLACK = 1;
  const Player NO_PLAYER = 2;

  inline std::string getUnicodePlayer(const Player player)
  {
    if(player==WHITE)
    {
      return "White";
    }
    if(player==BLACK)
    {
      return "Black";
    }
    if(player==NO_PLAYER)
    {
      return "-";
    }
    return "?";
  }

  typedef size_t Piece;

  const Piece PAWN = 0;
  const Piece KNIGHT = 1;
  const Piece BISHOP = 2;
  const Piece ROOK = 3;
  const Piece QUEEN = 4;
  const Piece KING = 5;
  const Piece NO_PIECE = 6;

  inline std::string getUnicodePiece(const Player player, const Piece piece)
  {
    if(player == BLACK)
    {
      switch(piece)
      {
        case PAWN: return "\u2659";
        case KNIGHT: return "\u2658";
        case BISHOP: return "\u2657";
        case ROOK: return "\u2656";
        case QUEEN: return "\u2655";
        case KING: return "\u2654";
        case NO_PIECE: return "-";
        default: return "?";
      }
    }
    if(player == WHITE)
    {
      switch(piece)
      {
        case PAWN: return "\u265F";
        case KNIGHT: return "\u265E";
        case BISHOP: return "\u265D";
        case ROOK: return "\u265C";
        case QUEEN: return "\u265B";
        case KING: return "\u265A";
        case NO_PIECE: return "-";
        default: return "?";
      }
    }
    if(player == NO_PLAYER)
    {
      return "-";
    }
    return "?";
  }

  inline std::string getNotation(const Player player, const Piece piece)
  {
    if(player == BLACK)
    {
      switch(piece)
      {
        case PAWN: return "p";
        case KNIGHT: return "n";
        case BISHOP: return "b";
        case ROOK: return "r";
        case QUEEN: return "q";
        case KING: return "k";
        case NO_PIECE: return "-";
        default: return "?";
      }
    }
    if(player == WHITE)
    {
      switch(piece)
      {
        case PAWN: return "P";
        case KNIGHT: return "N";
        case BISHOP: return "B";
        case ROOK: return "R";
        case QUEEN: return "Q";
        case KING: return "K";
        case NO_PIECE: return "-";
        default: return "?";
      }
    }
    if(player == NO_PLAYER)
    {
      return "-";
    }
    return "?";
  }

  inline std::tuple<Player, Piece> get_piece(const std::string s)
  {
    if(s=="p")
    {
      return std::tuple<Player, Piece>(BLACK, PAWN);
    }
    if(s=="n")
    {
      return std::tuple<Player, Piece>(BLACK, KNIGHT);
    }
    if(s=="b")
    {
      return std::tuple<Player, Piece>(BLACK, BISHOP);
    }
    if(s=="r")
    {
      return std::tuple<Player, Piece>(BLACK, ROOK);
    }
    if(s=="q")
    {
      return std::tuple<Player, Piece>(BLACK, QUEEN);
    }
    if(s=="k")
    {
      return std::tuple<Player, Piece>(BLACK, KING);
    }

    if(s=="P")
    {
      return std::tuple<Player, Piece>(WHITE, PAWN);
    }
    if(s=="N")
    {
      return std::tuple<Player, Piece>(WHITE, KNIGHT);
    }
    if(s=="B")
    {
      return std::tuple<Player, Piece>(WHITE, BISHOP);
    }
    if(s=="R")
    {
      return std::tuple<Player, Piece>(WHITE, ROOK);
    }
    if(s=="Q")
    {
      return std::tuple<Player, Piece>(WHITE, QUEEN);
    }
    if(s=="K")
    {
      return std::tuple<Player, Piece>(WHITE, KING);
    }
    return std::tuple<Player, Piece>(NO_PLAYER, NO_PIECE);
  }
}
