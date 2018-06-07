#pragma once

#include "chessTypes.hpp"

#include <array>

using namespace ChessTypes;

struct Move;

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
  void addPiece(Player player, Piece piece, uint64_t to);
  void removePiece(Player player, Piece piece, uint64_t from);
  void movePiece(Player player, Piece piece, uint64_t from, uint64_t to);
  std::string getString();
  uint64_t calculateZobristkey();
  uint64_t getUpdatedZobristkey(const Move& m);
  bool setFromFen(std::string fen);

  /*pub fn generate_move_list(&self) -> MoveList
  {
        let mut move_list = MoveList::empty_move_list();
        let new_en_passant_castling = self.en_passant_castling & (RANKS[0] | RANKS[7]);
        move_list.generate_pawn_moves(&self, new_en_passant_castling, false);
        move_list.generate_castling_moves(&self, new_en_passant_castling);
        move_list.generate_piece_moves(&self, KNIGHT, get_attack_mask_knight, new_en_passant_castling, false);
        move_list.generate_piece_moves(&self, BISHOP, get_attack_mask_bishop, new_en_passant_castling, false);
        move_list.generate_piece_moves(&self, ROOK, get_attack_mask_rook, new_en_passant_castling, false);
        move_list.generate_piece_moves(&self, QUEEN, get_attack_mask_queen, new_en_passant_castling, false);
        move_list.generate_piece_moves(&self, KING, get_attack_mask_king, new_en_passant_castling, false);
        move_list
    }
  pub fn generate_capture_move_list(&self) -> MoveList
  {
        let mut move_list = MoveList::empty_move_list();
        let new_en_passant_castling = self.en_passant_castling & (RANKS[0] | RANKS[7]);
        move_list.generate_pawn_moves(&self, new_en_passant_castling, true);
        move_list.generate_piece_moves(&self, KNIGHT, get_attack_mask_knight, new_en_passant_castling, true);
        move_list.generate_piece_moves(&self, BISHOP, get_attack_mask_bishop, new_en_passant_castling, true);
        move_list.generate_piece_moves(&self, ROOK, get_attack_mask_rook, new_en_passant_castling, true);
        move_list.generate_piece_moves(&self, QUEEN, get_attack_mask_queen, new_en_passant_castling, true);
        move_list.generate_piece_moves(&self, KING, get_attack_mask_king, new_en_passant_castling, true);
        move_list
    }
  pub fn is_check(&self, us: Player, enemy: Player, kings_index: usize) -> bool
  {
        let occupancy = self.players[WHITE] | self.players[BLACK];
        //QUEEN
        if get_attack_mask_queen(kings_index, occupancy) & self.pieces[QUEEN] & self.players[enemy] != 0
        {
            return true;
        }
        //KNIGHT
        if get_attack_mask_knight(kings_index, occupancy) & self.pieces[KNIGHT] & self.players[enemy] != 0
        {
            return true;
        }
        //BISHOP
        if get_attack_mask_bishop(kings_index, occupancy) & self.pieces[BISHOP] & self.players[enemy] != 0
        {
            return true;
        }
        //ROOK
        if get_attack_mask_rook(kings_index, occupancy) & self.pieces[ROOK] & self.players[enemy] != 0
        {
            return true;
        }
        //KING
        if get_attack_mask_king(kings_index, occupancy) & self.pieces[KING] & self.players[enemy] != 0
        {
            return true;
        }
        //PAWN
        if PAWN_CAPTURE_ATTACK_TABLE[us][kings_index] & self.pieces[PAWN] & self.players[enemy] != 0
        {
            return true;
        }
        false
    }
  pub fn is_check_unkown_kings_index(&self, us: Player, enemy: Player) -> bool
  {
        let kings_index = (self.pieces[KING] & self.players[us]).trailing_zeros() as usize;
        if kings_index == 64
        {
            return true;
        }
        self.is_check(us, enemy, kings_index)
    }
  pub fn make_move(&mut self, m: &Move)
  {
        let enemy = self.enemy;
        let us = self.us;
        self.en_passant_castling = m.en_passant_castling;
        //en passant
        if m.captured_en_passant
        {
            self.remove_piece(enemy, PAWN, PAWN_QUIET_ATTACK_TABLE[enemy][m.to]);
            self.move_piece(us, PAWN, BIT_AT_INDEX[m.from], BIT_AT_INDEX[m.to]);
        }
        //castling
        else if m.castled
        {
            //IF QUEENSIDE
            if m.to == CASTLING_QUEENSIDE_KING_TO_INDEX[us]
            {
                self.move_piece(us, KING, CASTLING_KING_FROM[us], CASTLING_QUEENSIDE_KING_TO[us]);
                self.move_piece(us, ROOK, CASTLING_QUEENSIDE_ROOK_FROM[us], CASTLING_QUEENSIDE_ROOK_TO[us]);
            }
            //IF KINGSIDE
            else
            {
                self.move_piece(us, KING, CASTLING_KING_FROM[us], CASTLING_KINGSIDE_KING_TO[us]);
                self.move_piece(us, ROOK, CASTLING_KINGSIDE_ROOK_FROM[us], CASTLING_KINGSIDE_ROOK_TO[us]);
            }
        }
        else
        {
            if m.captured != NO_PIECE
            {
                self.remove_piece(enemy, m.captured, BIT_AT_INDEX[m.to]);
            }
            if m.promoted == NO_PIECE
            {
                self.move_piece(us, m.moved, BIT_AT_INDEX[m.from], BIT_AT_INDEX[m.to]);
            }
            else
            {
                self.remove_piece(us, m.moved, BIT_AT_INDEX[m.from]);
                self.addPiece(us, m.promoted, BIT_AT_INDEX[m.to]);
            }
        }
        if m.moved == PAWN || m.captured != NO_PIECE
        {
            self.halfmove_clock = 0;
        }
        else
        {
            self.halfmove_clock += 1;
        }
        if self.us == BLACK
        {
            self.fullmoves_played += 1;
        }
        let temp = self.us;
        self.us = self.enemy;
        self.enemy = temp;
        self.zobrist_key = m.zobrist_key;
    }
  pub fn get_all_pseudo_legal_mov_string(&mut self) -> String
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
