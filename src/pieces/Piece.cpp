#include "Piece.h"
#include "Peon.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"

Piece::Piece(Position position, PIECE_COLOR color, PIECE_TYPE type): position(position), color(color), type(type) {
    std::string colorPrefix = color == PIECE_COLOR::C_WHITE ? "w" : "b";
    std::string pieceCharacter = GetPieceCharacterByType(type);

    this->name = colorPrefix + pieceCharacter;
}

void Piece::DoMove(const Move &move) {
    hasMoved = true;
    position = move.position;
}

Position Piece::GetPosition() {
    return position;
}

std::string Piece::GetName() {
    return name;
}

bool Piece::HasMoved() {
    return hasMoved;
}

Piece *Piece::CreatePieceByType(PIECE_TYPE type, const Position& position, PIECE_COLOR color) {
    switch (type) {
        case PEON:
            return new Peon(position, color);
        case ROOK:
            return new Rook(position, color);
        case KNIGHT:
            return new Knight(position, color);
        case BISHOP:
            return new Bishop(position, color);
        case QUEEN:
            return new Queen(position, color);
        case KING:
            return new King(position, color);
    }
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




