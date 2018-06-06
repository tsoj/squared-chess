#pragma once

#include "chessTypes.hpp"
#include "chessData.hpp"
#include "move.hpp"

#include <string>
#include <sstream>

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
  int fullmovesPlayed;
  int halfmoveClock;

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
  uint64_t calculateZobristkey()
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
    ret ^= enPassant_castling;
    ret ^= (uint64_t)us;
    return ret;
  }

  uint64_t getUpdatedZobristkey(const Move& m)
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

      ret ^= this->enPassant_castling;
      ret ^= m.enPassant_castling;
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
  bool setFromFen(std::string fen)
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

    std::cout << ">" << pieces << "<" << std::endl;
    std::cout << ">" << activeColor << "<" << std::endl;
    std::cout << ">" << castlingAvailability << "<" << std::endl;
    std::cout << ">" << enPassantTargetSquare << "<" << std::endl;
    std::cout << ">" << halfmoveClock << "<" << std::endl;
    std::cout << ">" << fullmoveNumber << "<" << std::endl;

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
          this->enPassant_castling |= CASTLING_KINGSIDE_ROOK_FROM[WHITE] | CASTLING_KING_FROM[WHITE];
          break;
        }
        case 'k':
        {
          this->enPassant_castling |= CASTLING_KINGSIDE_ROOK_FROM[BLACK] | CASTLING_KING_FROM[BLACK];
          break;
        }
        case 'Q':
        {
          this->enPassant_castling |= CASTLING_QUEENSIDE_ROOK_FROM[WHITE] | CASTLING_KING_FROM[WHITE];
          break;
        }
        case 'q':
        {
          this->enPassant_castling |= CASTLING_QUEENSIDE_ROOK_FROM[BLACK] | CASTLING_KING_FROM[BLACK];
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
      this->enPassant_castling |= BIT_AT_INDEX[enPassantTargetSquareIndex];
    }
    this->halfmoveClock = std::stoi(halfmoveClock);
    this->fullmovesPlayed = std::stoi(fullmoveNumber);
    zobristKey = calculateZobristkey();
    return true;
  }
  /*
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
