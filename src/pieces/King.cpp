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
        if (!board.At(position)) {
            possibleMoves.push_back({Move::TYPE::WALK, position});
        } else if (board.At(position) && board.At(position)->color != color) {
            possibleMoves.push_back({Move::TYPE::ATTACK, position});
        }
    }

    return possibleMoves;
}
