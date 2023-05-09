#include "Knight.h"

std::vector<Move> Knight::GetPossibleMoves(const Board &board) {
    std::vector<Position> possiblePositions = {
        // Up.
        {position.i - 2, position.j - 1},
        {position.i - 2, position.j + 1},

        // Right.
        {position.i - 1, position.j + 2},
        {position.i + 1, position.j + 2},

        // Down.
        {position.i + 2, position.j - 1},
        {position.i + 2, position.j + 1},

        // Left.
        {position.i - 1, position.j - 2},
        {position.i + 1, position.j - 2},
    };

    std::vector<Move> possibleMoves;

    for (const Position& position : possiblePositions) {
        if (board.IsPositionWithinBoundaries(position)) {
            if (!board.At(position)) {
                possibleMoves.push_back({MOVE_TYPE::WALK, position});
            } else if (board.At(position)->color != color) {
                possibleMoves.push_back({MOVE_TYPE::ATTACK, position});
            }
        }
    }

    return possibleMoves;
}
