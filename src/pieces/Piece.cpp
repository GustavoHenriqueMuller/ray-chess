#include "Piece.h"

void Piece::DoMove(const Position &move) {
    hasMoved = true;
    position = move;
}

Position Piece::GetPosition() {
    return position;
}
