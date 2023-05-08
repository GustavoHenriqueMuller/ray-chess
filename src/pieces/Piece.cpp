#include "Piece.h"

Piece::Piece(Position position, Piece::COLOR color, Piece::TYPE type): position(position), color(color), type(type) {
    std::string colorPrefix = color == Piece::COLOR::C_WHITE ? "w" : "b";
    std::string pieceCharacter = GetPieceCharacterByType(type);

    this->textureName = colorPrefix + pieceCharacter;
}

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

std::string Piece::GetPieceCharacterByType(Piece::TYPE type) {
    switch (type) {
        case PEON:
            return "p";
        case ROOK:
            return "r";
        case KNIGHT:
            return "n";
        case BISHOP:
            return "b";
        case QUEEN:
            return "q";
        case KING:
            return "k";
    }
}
