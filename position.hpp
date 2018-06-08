#pragma once

#include "chessTypes.hpp"

#include <array>

using namespace ChessTypes;

struct Move;
struct MoveList;

struct Position
{
  std::array<uint64_t, 6> pieces; // [Pawns, Knights, Bishops, Rooks, Queens, Kings]
  std::array<uint64_t, 2> players; // [White pieces, Black pieces]
  uint64_t enPassantCastling;
  uint64_t zobristKey;
  Player us;
  Player enemy;
  int fullmovesPlayed;
  int halfmoveClock;

  static Position emptyPosition();
  void addPiece(const Player player, const Piece piece, const uint64_t to);
  void removePiece(const Player player, const Piece piece, const uint64_t from);
  void movePiece(const Player player, const Piece piece, const uint64_t from, const uint64_t to);
  std::string getString() const;
  uint64_t calculateZobristKey() const;
  uint64_t getUpdatedZobristKey(const Move& m) const;
  bool setFromFen(const std::string fen);
  bool inCheck(const Player us, const Player enemy, const size_t kingsIndex) const;
  bool inCheck(const Player us, const Player enemy) const;
  MoveList generateMoveList() const;
  MoveList generateCaptureMoveList() const;
  void makeMove(Move m);
  /*pub fn get_all_pseudo_legal_mov_string(&mut self) -> String
  {
        let mut ret = "".to_string();
        let ml = self.generate_move_list();
        for i in 0..ml.len
        {
            let mut next_p = self.clone();
            next_p.make_move(&ml[i]);
            ret += "------------------------------------------------\n";
            ret += &next_p.get_chess_board_string()[..];
            ret += "\n";
            ret += &next_p.get_fen_string()[..];
            ret += "\n";
        }
        ret+= &ml.len.to_string()[..];
        ret += " pseudo-legal mov.\n";
        ret
    }
  */
};
