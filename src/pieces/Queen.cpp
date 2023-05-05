#include "Queen.h"

std::vector<Move> Queen::GetPossibleMoves(const Board &board) {
    std::vector<Move> moves;

    // Checking left.
    AddValidMoves(moves, {position.i, position.j - 1}, board, 0, -1);

    // Checking right.
    AddValidMoves(moves, {position.i, position.j + 1}, board, 0, 1);

    // Checking up.
    AddValidMoves(moves, {position.i - 1, position.j}, board, -1, 0);

    // Checking down.
    AddValidMoves(moves, {position.i + 1, position.j}, board, 1, 0);

    // Checking up-left.
    AddValidMoves(moves, {position.i - 1, position.j - 1}, board, -1, -1);

    // Checking up-right.
    AddValidMoves(moves, {position.i - 1, position.j + 1}, board, -1, 1);

    // Checking down-right.
    AddValidMoves(moves, {position.i + 1, position.j + 1}, board, 1, 1);

    // Checking down-left.
    AddValidMoves(moves, {position.i + 1, position.j - 1}, board, 1, -1);

    return moves;
}

void Queen::AddValidMoves(std::vector<Move> &moves, Position pos, const Board &board, int iIncrement, int jIncrement) {
    while (board.IsPositionWithinBoundaries(pos)) {
        if (!board.At(pos)) {
            moves.push_back({Move::TYPE::WALK, pos});

            pos.i += iIncrement;
            pos.j += jIncrement;
        } else if (board.At(pos) && board.At(pos)->type != type) {
            moves.push_back({Move::TYPE::ATTACK, pos});
            break;
        } else {
            break;
        }
    }
}
