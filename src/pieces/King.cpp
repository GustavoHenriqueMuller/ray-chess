#include "King.h"

std::vector<Position> King::GetPossibleMoves(const Board &board) {
    std::vector<Position> possibleMoves = {
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

    for (int i = possibleMoves.size() - 1; i >= 0; i--) {
        bool containsEmptySpaceOrEnemy = !board.At(possibleMoves[i]) || (board.At(possibleMoves[i]) && board.At(possibleMoves[i])->type != type);

        if (!(board.IsPositionWithinBoundaries(possibleMoves[i])) || !containsEmptySpaceOrEnemy) {
            possibleMoves.erase(possibleMoves.begin() + i);
        }
    }

    return possibleMoves;
}
