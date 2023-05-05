#include "Peon.h"

// TODO: PROMOTION

std::vector<Position> Peon::GetPossibleMoves(const Board& board) {
    std::vector<Position> moves;

    // If black, can only move down. Else, can only move up.
    Position baseMove = {position.i + (color == COLOR::C_BLACK ? +1 : -1), position.j};

    if (!board.At(baseMove)) {
        moves.push_back(baseMove);
    }

    // Check for moving two cells, if the peon has not been moved.
    Position twoCellMove = {position.i + (color == COLOR::C_BLACK ? +2 : -2), position.j};

    if (!board.At(baseMove) && !board.At(twoCellMove) && !this->hasMoved) {
        moves.push_back(twoCellMove);
    }

    // Check for attacks (diagonals).
    int attackRow = position.i + (color == COLOR::C_WHITE ? -1 : 1);

    Position attackLeft = {attackRow, position.j - 1};
    Position attackRight = {attackRow, position.j + 1};

    if (board.At(attackLeft) && board.At(attackLeft)->type != type) moves.push_back(attackLeft);
    if (board.At(attackRight) && board.At(attackRight)->type != type) moves.push_back(attackRight);

    // Check for en passant.
    Piece* pieceAtLeft = board.At({position.i, position.j - 1});

    if (!board.At(attackLeft) &&
        pieceAtLeft &&
        pieceAtLeft->type != type &&
        pieceAtLeft->type == Piece::TYPE::PEON
    ) {
        moves.push_back(attackLeft);
    }

    return moves;
}


