#include "Rook.h"

std::vector<Move> Rook::GetPossibleMoves(const Board& board) {
    std::vector<Move> moves;

    // Checking left.
    AddValidMoves(board, moves, {position.i, position.j - 1}, 0, -1);

    // Checking right.
    AddValidMoves(board, moves, {position.i, position.j + 1}, 0, 1);

    // Checking up.
    AddValidMoves(board, moves, {position.i - 1, position.j}, -1, 0);

    // Checking down.
    AddValidMoves(board, moves, {position.i + 1, position.j}, 1, 0);

    return moves;
}

void Rook::AddValidMoves(const Board& board, std::vector<Move>& moves, Position pos, int iIncrement, int jIncrement) {
    while (board.IsPositionWithinBoundaries(pos)) {
        if (!board.At(pos)) {
            moves.push_back({MOVE_TYPE::WALK, pos});

            pos.i += iIncrement;
            pos.j += jIncrement;
        } else if (board.At(pos) && board.At(pos)->color != color) {
            moves.push_back({MOVE_TYPE::ATTACK, pos});
            break;
        } else {
            break;
        }
    }
}


