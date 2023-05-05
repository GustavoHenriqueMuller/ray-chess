#include "Piece.h"

void Piece::DoMove(const Move &move) {
    hasMoved = true;
    position = move.position;
}

Position Piece::GetPosition() {
    return position;
}

bool Piece::HasMoved() {
    return hasMoved;
}

Piece::COLOR Piece::GetInverseColor(Piece::COLOR color) {
    return color == Piece::COLOR::C_WHITE ? Piece::COLOR::C_BLACK : Piece::COLOR::C_WHITE;
}
