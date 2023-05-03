#include "Peon.h"

// TODO: EN PASSANT

void Peon::Move(const Position &move) {
    hasMoved = true;
    position = move;
}

std::vector<Position> Peon::GetPossibleMoves(const Board& board) {
    std::vector<Position> moves;

    // If black, can only move down. Else, can only move up.
    Position baseMove = {position.i + (color == PIECE_COLOR::PIECE_BLACK ? +1 : -1), position.j};

    if (!board.At(baseMove)) {
        moves.push_back(baseMove);
    }

    // Check for moving two cells, if the peon has not been moved.
    Position twoCellMove = {position.i + (color == PIECE_COLOR::PIECE_BLACK ? +2 : -2), position.j};

    if (!board.At(twoCellMove) && !this->hasMoved) {
        moves.push_back(twoCellMove);
    }

    // Check for attacks.
    int attackRow = position.i + (color == PIECE_WHITE ? -1 : 1);

    Position attackLeft = {attackRow, position.j - 1};
    Position attackRight = {attackRow, position.j + 1};

    if (board.At(attackLeft) && board.At(attackLeft)->color != color) moves.push_back(attackLeft);
    if (board.At(attackRight) && board.At(attackRight)->color != color) moves.push_back(attackRight);

    return moves;
}


