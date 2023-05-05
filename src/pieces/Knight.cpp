#include "Knight.h"

std::vector<Position> Knight::GetPossibleMoves(const Board &board) {
    std::vector<Position> possibleMoves = {
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

    for (int i = possibleMoves.size() - 1; i >= 0; i--) {
        bool containsEmptySpaceOrEnemy = !board.At(possibleMoves[i]) || (board.At(possibleMoves[i]) && board.At(possibleMoves[i])->type != type);

        if (!(board.IsPositionWithinBoundaries(possibleMoves[i])) || !containsEmptySpaceOrEnemy) {
            possibleMoves.erase(possibleMoves.begin() + i);
        }
    }

    return possibleMoves;
}
