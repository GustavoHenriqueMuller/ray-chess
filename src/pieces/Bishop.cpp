#include "Bishop.h"

std::vector<Move> Bishop::GetPossibleMoves(const Board& board) {
    std::vector<Move> moves;

    // Checking up-left.
    AddValidMoves(board, moves, {position.i - 1, position.j - 1}, -1, -1);

    // Checking up-right.
    AddValidMoves(board, moves, {position.i - 1, position.j + 1}, -1, 1);

    // Checking down-right.
    AddValidMoves(board, moves, {position.i + 1, position.j + 1}, 1, 1);

    // Checking down-left.
    AddValidMoves(board, moves, {position.i + 1, position.j - 1}, 1, -1);

    return moves;
}

void Bishop::AddValidMoves(const Board& board, std::vector<Move>& moves, Position pos, int iIncrement, int jIncrement) {
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
