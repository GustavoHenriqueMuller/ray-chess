#include "Peon.h"

// TODO: EN PASSANT
// TODO: PERMITIR MOVER DUAS CASAS

std::vector<Position> Peon::GetPossibleMoves(const Board& board) {
    std::vector<Position> moves;

    // If black, can only move down. Else, can only move up.
    Position baseMove;

    if (color == PIECE_COLOR::PIECE_BLACK) {
        baseMove = Position{position.i + 1, position.j};
    } else {
        baseMove = Position{position.i - 1, position.j};
    }

    if (!board.At(baseMove)) {
        moves.push_back(baseMove);
    }

    // Check for attacks.
    int attackRow = color == PIECE_WHITE ? -1 : 1;

    Position attackLeft = {position.i + attackRow, position.j - 1};
    Position attackRight = {position.i + attackRow, position.j + 1};

    if (board.At(attackLeft) && board.At(attackLeft)->color != color) moves.push_back(attackLeft);
    if (board.At(attackRight) && board.At(attackRight)->color != color) moves.push_back(attackRight);

    return moves;
}
