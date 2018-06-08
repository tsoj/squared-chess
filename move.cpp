#include "move.hpp"

std::vector<Move> MoveList::movePool = std::vector<Move>();
std::vector<size_t> MoveList::freeOffsets = {0};

template<>
void MoveList::generateMoves<PAWN>(const Position& origPosition, const uint64_t newEnPassantCastling, const bool onlyCaptures)
{
  const Player enemy = origPosition.enemy;
  const Player us = origPosition.us;
  uint64_t pawnOccupancy = origPosition.pieces[PAWN] & origPosition.players[us];
  const uint64_t occupancy = origPosition.players[WHITE] | origPosition.players[BLACK];
  if(pawnOccupancy != 0)
  {
    while(pawnOccupancy != 0)
    {
      const size_t from = findAndClearTrailingOne(pawnOccupancy);

      // quiet single and double push moves
      if((PAWN_QUIET_ATTACK_TABLE[us][from] & occupancy) == 0)
      {
        const size_t to = trailingZeros(PAWN_QUIET_ATTACK_TABLE[us][from]);
        if((BIT_AT_INDEX[to] & HOME_RANK[enemy]) != 0)
        {
          this->addMove(from, to, PAWN, NO_PIECE, KNIGHT, newEnPassantCastling, false, false, origPosition);
          this->addMove(from, to, PAWN, NO_PIECE, BISHOP, newEnPassantCastling, false, false, origPosition);
          this->addMove(from, to, PAWN, NO_PIECE, ROOK, newEnPassantCastling, false, false, origPosition);
          this->addMove(from, to, PAWN, NO_PIECE, QUEEN, newEnPassantCastling, false, false, origPosition);
        }
        else if(not onlyCaptures)
        {
          this->addMove(from, to, PAWN, NO_PIECE, NO_PIECE, newEnPassantCastling, false, false, origPosition);
          if((BIT_AT_INDEX[from] & PAWN_HOME_RANK[us]) != 0)
          {
            const size_t doublePushTo = trailingZeros(PAWN_QUIET_ATTACK_TABLE[us][to]);
            if((BIT_AT_INDEX[doublePushTo] & occupancy) == 0)
            {
              this->addMove(
                from,
                doublePushTo,
                PAWN,
                NO_PIECE,
                NO_PIECE,
                newEnPassantCastling | BIT_AT_INDEX[to],
                false,
                false,
                origPosition
              );
            }
          }
        }
      }

      // capture moves
      uint64_t captureAttackMask = PAWN_CAPTURE_ATTACK_TABLE[us][from] & origPosition.players[enemy];
      if(captureAttackMask != 0)
      {
        while(captureAttackMask != 0)
        {
          const size_t to = findAndClearTrailingOne(captureAttackMask);
          for(Piece captured = 0;  captured<NO_PIECE; captured++)
          {
            if((origPosition.pieces[captured] & BIT_AT_INDEX[to]) != 0)
            {
              if((BIT_AT_INDEX[to] & HOME_RANK[enemy]) != 0)
              {
                const uint64_t updatedNewEnPassantCastling = newEnPassantCastling & ~BIT_AT_INDEX[to];
                this->addMove(from, to, PAWN, captured, KNIGHT, updatedNewEnPassantCastling, false, false, origPosition);
                this->addMove(from, to, PAWN, captured, BISHOP, updatedNewEnPassantCastling, false, false, origPosition);
                this->addMove(from, to, PAWN, captured, ROOK, updatedNewEnPassantCastling, false, false, origPosition);
                this->addMove(from, to, PAWN, captured, QUEEN, updatedNewEnPassantCastling, false, false, origPosition);
              }
              else
              {
                this->addMove(from, to, PAWN, captured, NO_PIECE, newEnPassantCastling, false, false, origPosition);
              }
              break;
            }
          }
        }
      }

      // en-passant
      captureAttackMask = PAWN_CAPTURE_ATTACK_TABLE[us][from] & origPosition.enPassantCastling & (RANKS[2] | RANKS[5]);
      if(captureAttackMask != 0)
      {
          const size_t to = trailingZeros(captureAttackMask);
          this->addMove(from, to, PAWN, PAWN, NO_PIECE, newEnPassantCastling, false, true, origPosition);
      }
    }
  }
}

void MoveList::generateCastlingMoves(const Position& origPosition, uint64_t newEnPassantCastling)
{
  const Player enemy = origPosition.enemy;
  const Player us = origPosition.us;

  if((origPosition.enPassantCastling & CASTLING_KING_FROM[us]) != 0)
  {
    const uint64_t occupancy = origPosition.players[WHITE] | origPosition.players[BLACK];
    // queenside castling
    if(
      origPosition.enPassantCastling & CASTLING_QUEENSIDE_ROOK_FROM[us] != 0 &&
      CASTLING_QUEENSIDE_BLOCK_RELEVANT_AREA[us] & occupancy == 0 &&
      not origPosition.inCheck(us, enemy, CASTLING_QUEENSIDE_CHECK_RELEVANT_FIELDS[us][0]) &&
      not origPosition.inCheck(us, enemy, CASTLING_QUEENSIDE_CHECK_RELEVANT_FIELDS[us][1])
    )
    {
      this->addMove(
        CASTLING_KING_FROM_INDEX[us],
        CASTLING_QUEENSIDE_KING_TO_INDEX[us],
        KING,
        NO_PIECE,
        NO_PIECE,
        newEnPassantCastling & ~(CASTLING_KING_FROM[us] | CASTLING_QUEENSIDE_ROOK_FROM[us]),
        true,
        false,
        origPosition
      );
    }
    // kingside castling
    if(
      origPosition.enPassantCastling & CASTLING_KINGSIDE_ROOK_FROM[us] != 0 &&
      CASTLING_KINGSIDE_BLOCK_RELEVANT_AREA[us] & occupancy == 0 &&
      not origPosition.inCheck(us, enemy, CASTLING_KINGSIDE_CHECK_RELEVANT_FIELDS[us][0]) &&
      not origPosition.inCheck(us, enemy, CASTLING_KINGSIDE_CHECK_RELEVANT_FIELDS[us][1])
    )
    {
      this->addMove(
        CASTLING_KING_FROM_INDEX[us],
        CASTLING_KINGSIDE_KING_TO_INDEX[us],
        KING,
        NO_PIECE,
        NO_PIECE,
        newEnPassantCastling & ~(CASTLING_KING_FROM[us] | CASTLING_KINGSIDE_ROOK_FROM[us]),
        true,
        false,
        origPosition
      );
    }
  }
}
