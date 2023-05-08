#include "Piece.h"

Piece::Piece(Position position, PIECE_COLOR color, PIECE_TYPE type): position(position), color(color), type(type) {
    std::string colorPrefix = color == PIECE_COLOR::C_WHITE ? "w" : "b";
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

std::string Piece::GetTextureName() {
    return textureName;
}

bool Piece::HasMoved() {
    return hasMoved;
}

PIECE_COLOR Piece::GetInverseColor(PIECE_COLOR color) {
    return color == PIECE_COLOR::C_WHITE ? PIECE_COLOR::C_BLACK : PIECE_COLOR::C_WHITE;
}

std::string Piece::GetPieceCharacterByType(PIECE_TYPE type) {
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


