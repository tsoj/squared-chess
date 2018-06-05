#pragma once

#include "chessTypes.hpp"
#include "chessData.hpp"

#include <string>

using namespace ChessTypes;
using namespace ChessData;

struct Position
{
  uint64_t pieces[6]; // [Pawns, Knights, Bishops, Rooks, Queens, Kings]
  uint64_t players[2]; // [White pieces, Black pieces]
  uint64_t enPassant_castling;
  uint64_t zobristKey;
  Player us;
  Player enemy;
  uint16_t fullmovesPlayed;
  uint16_t halfmoveClock;

  static Position emptyPosition()
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
  void addPiece(Player player, Piece piece, uint64_t to)
  {
    this->pieces[piece] |=  to;
    this->players[player] |=  to;
  }
  void removePiece(Player player, Piece piece, uint64_t from)
  {
    this->pieces[piece] &=  !from;
    this->players[player] &=  !from;
  }
  void movePiece(Player player, Piece piece, uint64_t from, uint64_t to)
  {
    this->removePiece(player, piece, from);
    this->addPiece(player, piece, to);
  }
  std::string getString()
  {
    std::string ret = "";
    for(size_t rank = 0; rank<8; rank++)
    {
      for(size_t file = 7; file<=7; file--)
      {
        bool foundPiece = false;
        for(Player player = 0; player<NO_PLAYER; player++)
        {
          for(Piece piece = 0; piece<NO_PIECE; piece++)
          {
            if((BIT_AT_INDEX[8*rank + file] & this->pieces[piece] & this->players[player]) != 0)
            {
              ret = getUnicodePiece(player, piece) + ret;
              foundPiece = true;
            }
          }
        }
        if(foundPiece == false)
        {
          ret = " " + ret;
        }
      }
    }
    return ret;
  }
  /*pub fn get_fen_string(&self) -> String
  {
      let mut temp: Vec<String> = vec![String::new(); 64];
      for  i in 0..BIT_AT_INDEX.len()
      {
            temp[i] = "1".to_string();
            if (self.players[BLACK] & BIT_AT_INDEX[i]) != 0
            {
                if (self.pieces[PAWN] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "p".to_string();
                }
                else if (self.pieces[KNIGHT] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "n".to_string();
                }
                else if (self.pieces[BISHOP] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "b".to_string();
                }
                else if (self.pieces[ROOK] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "r".to_string();
                }
                else if (self.pieces[QUEEN] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "q".to_string();
                }
                else if (self.pieces[KING] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "k".to_string();
                }
            }
            else if (self.players[WHITE] & BIT_AT_INDEX[i]) != 0
            {
                if (self.pieces[PAWN] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "P".to_string();
                }
                else if (self.pieces[KNIGHT] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "N".to_string();
                }
                else if (self.pieces[BISHOP] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "B".to_string();
                }
                else if (self.pieces[ROOK] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "R".to_string();
                }
                else if (self.pieces[QUEEN] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "Q".to_string();
                }
                else if (self.pieces[KING] & BIT_AT_INDEX[i]) != 0
                {
                    temp[i] = "K".to_string();
                }
            }
        }
      let mut fen = format_for_fen(&temp);
      if self.us == WHITE
      {
            fen.push_str(" w ");
        }
      else
      {
            fen.push_str(" b ");
        }
      let castling = self.en_passant_castling & (RANKS[0] | RANKS[7]);
      if castling != 0
      {
            if castling & CASTLING_KING_FROM[WHITE] != 0 &&
               castling & CASTLING_KINGSIDE_ROOK_FROM[WHITE] != 0
            {
                fen.push_str("K");
            }
            if castling & CASTLING_KING_FROM[WHITE] != 0 &&
               castling & CASTLING_QUEENSIDE_ROOK_FROM[WHITE] != 0
            {
                fen.push_str("Q");
            }
            if castling & CASTLING_KING_FROM[BLACK] != 0 &&
               castling & CASTLING_KINGSIDE_ROOK_FROM[BLACK] != 0
            {
                fen.push_str("k");
            }
            if castling & CASTLING_KING_FROM[BLACK] != 0 &&
               castling & CASTLING_QUEENSIDE_ROOK_FROM[BLACK] != 0
            {
                fen.push_str("q");
            }
            fen.push_str(" ");
        }
      else
      {
            fen.push_str("- ");
        }
      let en_passant = self.en_passant_castling & !castling;
      if en_passant != 0
      {
            fen.push_str(get_field_notation(find_and_clear_trailing_one(&mut en_passant.clone())));
        }
      else
      {
            fen.push_str("-");
        }
      fen.push_str(" ");
      fen.push_str(&(self.halfmove_clock).to_string());
      fen.push_str(" ");
      fen.push_str(&(self.fullmoves_played).to_string());
      fen
  }
  pub fn get_data_string(&self) -> String
  {
        let mut ret = "".to_string();
        ret += "\nWHOSE MOVE: ";
        ret += &self.us.to_string()[..];
        ret += "\nFULLMOVES PLAYED: ";
        ret += &self.fullmoves_played.to_string()[..];
        ret += "\nHALFMOVE CLOCK: ";
        ret += &self.halfmove_clock.to_string()[..];
        ret += "\nZOBRIST KEY: ";
        ret += &format!("{:x}", self.zobrist_key)[..];
        ret += "\nCASTLING / EN PASSANT\n";
        ret += &get_bitboard_string(self.en_passant_castling)[..];
        ret += "WHITE:\n";
        ret += &get_bitboard_string(self.players[WHITE])[..];
        ret += "BLACK:\n";
        ret += &get_bitboard_string(self.players[BLACK])[..];
        ret += "PAWNS:\n";
        ret += &get_bitboard_string(self.pieces[PAWN])[..];
        ret += "KNIGHTS:\n";
        ret += &get_bitboard_string(self.pieces[KNIGHT])[..];
        ret += "BISHOPS:\n";
        ret += &get_bitboard_string(self.pieces[BISHOP])[..];
        ret += "ROOKS:\n";
        ret += &get_bitboard_string(self.pieces[ROOK])[..];
        ret += "QUEENS:\n";
        ret += &get_bitboard_string(self.pieces[QUEEN])[..];
        ret += "KINGS:\n";
        ret += &get_bitboard_string(self.pieces[KING])[..];
        ret
    }
  pub fn set_from_fen(&mut self, fen: &String) -> bool
  {
        let mut p = Position::empty_position();
        let mut iter = (*fen).split_whitespace();
        let piece_placement = iter.next().unwrap().to_string();
        let active_color = iter.next().unwrap().to_string();
        let castling_availability = iter.next().unwrap().to_string();
        let en_passant_target_square = iter.next().unwrap().to_string();
        let halfmove_clock = iter.next().unwrap().to_string();//fifty-move rule
        let fullmove_number = iter.next().unwrap().to_string();

        let mut field_counter: usize = 56;
        for i in piece_placement.chars()
        {
            match i
            {
                '/' => field_counter-=16,
                '8' => field_counter+=8,
                '7' => field_counter+=7,
                '6' => field_counter+=6,
                '5' => field_counter+=5,
                '4' => field_counter+=4,
                '3' => field_counter+=3,
                '2' => field_counter+=2,
                '1' => field_counter+=1,
                '0' => field_counter+=0,
                'P' =>
                {
                    p.add_piece(WHITE, PAWN, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'N' =>
                {
                    p.add_piece(WHITE, KNIGHT, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'B' =>
                {
                    p.add_piece(WHITE, BISHOP, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'R' =>
                {
                    p.add_piece(WHITE, ROOK, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'Q' =>
                {
                    p.add_piece(WHITE, QUEEN, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'K' =>
                {
                    p.add_piece(WHITE, KING, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'p' =>
                {
                    p.add_piece(BLACK, PAWN, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'n' =>
                {
                    p.add_piece(BLACK, KNIGHT, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'b' =>
                {
                    p.add_piece(BLACK, BISHOP, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'r' =>
                {
                    p.add_piece(BLACK, ROOK, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'q' =>
                {
                    p.add_piece(BLACK, QUEEN, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                'k' =>
                {
                    p.add_piece(BLACK, KING, BIT_AT_INDEX[field_counter]);
                    field_counter+=1;
                },
                _x =>
                {
                    println!("FEN-string not formatted properly.");
                    return false;
                }
            }
        }
        if active_color == "w" || active_color == "W"
        {
            p.us = WHITE;
            p.enemy = BLACK;
        }
        else if active_color == "b" || active_color == "B"
        {
            p.us = BLACK;
            p.enemy = WHITE;
        }
        else
        {
            println!("FEN-string not formatted properly.");
            return false;
        }
        for i in castling_availability.chars()
        {
            match i
            {
                '-' => {},
                'K' => p.en_passant_castling |= CASTLING_KINGSIDE_ROOK_FROM[WHITE] | CASTLING_KING_FROM[WHITE],
                'k' => p.en_passant_castling |= CASTLING_KINGSIDE_ROOK_FROM[BLACK] | CASTLING_KING_FROM[BLACK],
                'Q' => p.en_passant_castling |= CASTLING_QUEENSIDE_ROOK_FROM[WHITE] | CASTLING_KING_FROM[WHITE],
                'q' => p.en_passant_castling |= CASTLING_QUEENSIDE_ROOK_FROM[BLACK] | CASTLING_KING_FROM[BLACK],
                _x =>
                {
                    println!("FEN-string not formatted properly.");
                    return false;
                }
            }
        }
        if en_passant_target_square!="-"
        {
            let en_passant_target_field_index = get_field_index(&en_passant_target_square[..]);
            if en_passant_target_field_index == 64
            {
                println!("FEN-string not formatted properly.");
                return false;
            }
            p.en_passant_castling |= BIT_AT_INDEX[en_passant_target_field_index];
        }
        p.halfmove_clock = halfmove_clock.parse::<u16>().unwrap();
        p.fullmoves_played = fullmove_number.parse::<u16>().unwrap();
        p.zobrist_key = p.calculate_zobristkey();
        self.clone_from(&p);
        true
    }
  pub fn calculate_zobristkey(&self) -> u64
  {
        let mut ret: u64 = 0;
        for i in 0..NO_PIECE
        {
            if self.pieces[i] != 0
            {
                let mut temp_occupancy = self.pieces[i];
                loop
                {
                    let field_index = temp_occupancy.trailing_zeros() as usize;
                    if (BIT_AT_INDEX[field_index] & self.players[WHITE])!=0
                    {
                        ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[WHITE][field_index];
                    }
                    else
                    {
                        ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[BLACK][field_index];
                    }
                    ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[i][field_index];
                    temp_occupancy &= !BIT_AT_INDEX[field_index];
                    if temp_occupancy == 0
                    {
                        break;
                    }
                }
            }
        }
        ret ^= self.en_passant_castling;
        ret ^= self.us as u64;
        ret
    }
  pub fn get_updated_zobristkey(&self, m: &Move) -> u64
  {
        let mut ret: u64 = self.zobrist_key;

        ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.moved][m.from];
        if m.promoted != NO_PIECE
        {
            ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.promoted][m.to];
        }
        else
        {
            ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.moved][m.to];
        }

        ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.us][m.from];
        ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.us][m.to];

        if m.captured != NO_PIECE && !m.captured_en_passant
        {
            ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[m.captured][m.to];
            ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.enemy][m.to];
        }

        ret ^= self.en_passant_castling;
        ret ^= m.en_passant_castling;
        ret ^= WHITE as u64;
        ret ^= BLACK as u64;

        if m.captured_en_passant
        {
            let captured_index = PAWN_QUIET_ATTACK_TABLE[self.enemy][m.to].trailing_zeros() as usize;
            ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[PAWN][captured_index];
            ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.enemy][captured_index];
        }

        if m.castled
        {
            //IF QUEENSIDE
            if m.to == CASTLING_QUEENSIDE_KING_TO_INDEX[self.us]
            {
                ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_QUEENSIDE_ROOK_FROM_INDEX[self.us]];
                ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_QUEENSIDE_ROOK_TO_INDEX[self.us]];
                ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.us][CASTLING_QUEENSIDE_ROOK_FROM_INDEX[self.us]];
                ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.us][CASTLING_QUEENSIDE_ROOK_TO_INDEX[self.us]];
            }
            //IF KINGSIDE
            else
            {
                ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_KINGSIDE_ROOK_FROM_INDEX[self.us]];
                ret ^= ZOBRIST_RANDOM_BITMASKS_PIECES[ROOK][CASTLING_KINGSIDE_ROOK_TO_INDEX[self.us]];
                ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.us][CASTLING_KINGSIDE_ROOK_FROM_INDEX[self.us]];
                ret ^= ZOBRIST_RANDOM_BITMASKS_PLAYERS[self.us][CASTLING_KINGSIDE_ROOK_TO_INDEX[self.us]];
            }
        }
        ret
    }
  pub fn generate_move_list(&self) -> MoveList
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
                self.add_piece(us, m.promoted, BIT_AT_INDEX[m.to]);
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
