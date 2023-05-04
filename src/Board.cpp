#include "Board.h"

Board::Board() {
    // Init board.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pieces[i][j] = nullptr;
        }
    }
}

Board::~Board() {
    // Free board.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (pieces[i][j] != nullptr) {
                delete pieces[i][j]; // @TODO: VER
            }
        }
    }
}

Piece* Board::At(const Position& position) const {
    return pieces[position.i][position.j];
}

void Board::Set(const Position& position, Piece *piece) {
    pieces[position.i][position.j] = piece;
}

void Board::Destroy(const Position& position) {
    delete pieces[position.i][position.j]; // @TODO: VER
    pieces[position.i][position.j] = nullptr;
}

bool Board::IsPositionWithinBoundaries(const Position &position) const {
    return position.j >= 0 && position.j < 8 && position.i >= 0 && position.i < 8;
}
