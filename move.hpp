#pragma once

#include "chessTypes.hpp"
#include "chessData.hpp"

using namespace ChessTypes;
using namespace ChessData;

struct Move
{
    size_t from;
    size_t to;
    Piece moved;
    Piece captured;
    Piece promoted;
    uint64_t enPassant_castling;
    uint64_t zobristKey;
    bool castled;
    bool capturedEnPassant;
    Score score;
};
