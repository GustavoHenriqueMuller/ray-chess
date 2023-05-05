#include "Piece.h"

void Piece::DoMove(const Move &move) {
    hasMoved = true;
    position = move.position;
}

Position Piece::GetPosition() {
    return position;
}
