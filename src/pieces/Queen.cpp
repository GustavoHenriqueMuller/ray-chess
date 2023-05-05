#include "Queen.h"

std::vector<Position> Queen::GetPossibleMoves(const Board &board) {
    std::vector<Position> moves;

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

void Queen::AddValidMoves(std::vector<Position> &moves, Position move, const Board &board, int iIncrement, int jIncrement) {
    while (board.IsPositionWithinBoundaries(move)) {
        if (!board.At(move)) {
            moves.push_back(move);

            move.i += iIncrement;
            move.j += jIncrement;
        } else if (board.At(move) && board.At(move)->type != type) {
            moves.push_back(move);
            break;
        } else {
            break;
        }
    }
}
