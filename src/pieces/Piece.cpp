#include "Piece.h"

void Piece::Move(const Position &move) {
    hasMoved = true;
    position = move;
}

Position Piece::GetPosition() {
    return position;
}
