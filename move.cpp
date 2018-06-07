#include "move.hpp"

std::vector<Move> MoveList::movePool = std::vector<Move>();
std::vector<size_t> MoveList::freeOffsets = std::vector<size_t>();

template<>
void MoveList::generateMoves<PAWN>()
{
  /*const Player enemy = origPosition.enemy;
  const Player us = origPosition.us;
  uint64_t pawnOccupancy = origPosition.pieces[PAWN] & origPosition.players[us];
  uint64_t occupancy = origPosition.players[WHITE] | origPosition.players[BLACK];
  /*
      if pawn_occupancy != 0
      {
          loop
          {
              let from = find_and_clear_trailing_one(&mut pawn_occupancy) as usize;

              if PAWN_QUIET_ATTACK_TABLE[us][from] & occupancy == 0
              {
                  let to = PAWN_QUIET_ATTACK_TABLE[us][from].trailing_zeros() as usize;
                  if BIT_AT_INDEX[to] & HOME_RANK[enemy] != 0
                  {
                      self.add_move(from, to, PAWN, NO_PIECE, KNIGHT, new_en_passant_castling, false, false, &orig_position);
                      self.add_move(from, to, PAWN, NO_PIECE, BISHOP, new_en_passant_castling, false, false, &orig_position);
                      self.add_move(from, to, PAWN, NO_PIECE, ROOK, new_en_passant_castling, false, false, &orig_position);
                      self.add_move(from, to, PAWN, NO_PIECE, QUEEN, new_en_passant_castling, false, false, &orig_position);
                  }
                  else if !only_captures
                  {
                      self.add_move(from, to, PAWN, NO_PIECE, NO_PIECE, new_en_passant_castling, false, false, &orig_position);
                      if BIT_AT_INDEX[from] & PAWN_HOME_RANK[us] != 0
                      {
                          let double_push_to = PAWN_QUIET_ATTACK_TABLE[us][to].trailing_zeros() as usize;
                          if BIT_AT_INDEX[double_push_to] & occupancy == 0
                          {
                              self.add_move(
                                  from,
                                  double_push_to,
                                  PAWN,
                                  NO_PIECE,
                                  NO_PIECE,
                                  new_en_passant_castling | BIT_AT_INDEX[to],
                                  false,
                                  false,
                                  &orig_position
                              );
                          }
                      }
                  }
              }
              let mut capture_attack_mask = PAWN_CAPTURE_ATTACK_TABLE[us][from] & orig_position.players[enemy];
              if capture_attack_mask != 0
              {
                  loop
                  {
                      let to = find_and_clear_trailing_one(&mut capture_attack_mask);
                      for i in 0..NO_PIECE
                      {
                          if (orig_position.pieces[i] & BIT_AT_INDEX[to]) != 0
                          {
                              if BIT_AT_INDEX[to] & HOME_RANK[enemy] != 0
                              {
                                  let n_new_en_passant_castling = new_en_passant_castling & !BIT_AT_INDEX[to];
                                  self.add_move(from, to, PAWN, i as Piece, KNIGHT, n_new_en_passant_castling, false, false, &orig_position);
                                  self.add_move(from, to, PAWN, i as Piece, BISHOP, n_new_en_passant_castling, false, false, &orig_position);
                                  self.add_move(from, to, PAWN, i as Piece, ROOK, n_new_en_passant_castling, false, false, &orig_position);
                                  self.add_move(from, to, PAWN, i as Piece, QUEEN, n_new_en_passant_castling, false, false, &orig_position);
                              }
                              else
                              {
                                  self.add_move(from, to, PAWN, i as Piece, NO_PIECE, new_en_passant_castling, false, false, &orig_position);
                              }
                              break;
                          }
                      }

                      if capture_attack_mask == 0
                      {
                          break;
                      }
                  }
              }
              capture_attack_mask = PAWN_CAPTURE_ATTACK_TABLE[us][from] & orig_position.en_passant_castling & (RANKS[2] | RANKS[5]);
              if capture_attack_mask != 0
              {
                  let to = capture_attack_mask.trailing_zeros() as usize;
                  self.add_move(from, to, PAWN, PAWN, NO_PIECE, new_en_passant_castling, false, true, &orig_position);
              }

              if pawn_occupancy == 0
              {
                  break;
              }
          }
      }*/
  std::cout << "hi 1" << std::endl;
}
