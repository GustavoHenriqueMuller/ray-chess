#include "King.h"

std::vector<Move> King::GetPossibleMoves(const Board &board) {
    std::vector<Position> possiblePositions = {
            // Up.
            {position.i - 1, position.j},
            // Down.
            {position.i + 1, position.j},
            // Right.
            {position.i, position.j + 1},
            // Left.
            {position.i, position.j - 1},
            // Up-left.
            {position.i - 1, position.j - 1},
            // Up-right.
            {position.i - 1, position.j + 1},
            // Down-left.
            {position.i + 1, position.j - 1},
            // Down-right.
            {position.i + 1, position.j + 1},
    };

    std::vector<Move> possibleMoves;

    for (const Position& position : possiblePositions) {
        if (board.IsPositionWithinBoundaries(position)) {
            if (!board.At(position)) {
                possibleMoves.push_back({MOVE_TYPE::WALK, position});
            } else if (board.At(position) && board.At(position)->color != color) {
                possibleMoves.push_back({MOVE_TYPE::ATTACK, position});
            }
        }
    }

    // Check for long castling (left rook).
    if (CheckCastling(board, {position.i, 0})) {
        possibleMoves.push_back({MOVE_TYPE::LONG_CASTLING, {position.i, 2}});
    }

    // Check for short castling (right rook).
    if (CheckCastling(board, {position.i, 7})) {
        possibleMoves.push_back({MOVE_TYPE::SHORT_CASTLING, {position.i, 6}});
    }

    // Remove all moves that go out of the board.
    for (int i = possibleMoves.size() - 1; i >= 0; i--) {
        if (!board.IsPositionWithinBoundaries(possibleMoves[i].position)) {
            possibleMoves.erase(possibleMoves.begin() + i);
        }
    }

    return possibleMoves;
}

bool King::CheckCastling(const Board &board, const Position &position) {
    Piece* piece = board.At(position);

    if (piece && piece->color == color && piece->type == PIECE_TYPE::ROOK && !piece->HasMoved() && !hasMoved) {
        //for ()

        // TODO: Check for spaces between the king and the rook.
        return true;
    }

    return false;
}
