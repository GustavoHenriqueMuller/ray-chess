#ifndef RAY_CHESS_PIECE_H
#define RAY_CHESS_PIECE_H

class Piece; // Forward declaration (circular dependency).

#include "raylib.h"
#include "../Position.h"
#include "../Board.h"
#include "../Move.h"

#include <vector>

class Piece {
public:
    enum TYPE {
        PEON,
        ROOK,
        KNIGHT,
        BISHOP,
        QUEEN,
        KING
    };

    enum COLOR {
        C_WHITE,
        C_BLACK
    };

    Piece(Position position, Piece::COLOR color, Piece::TYPE type);

    static Piece::COLOR GetInverseColor(Piece::COLOR color);
    static std::string GetPieceCharacterByType(Piece::TYPE type);

    virtual void DoMove(const Move& move);
    virtual std::vector<Move> GetPossibleMoves(const Board& board) = 0;

    Position GetPosition();
    bool HasMoved();

    const COLOR color;
    const TYPE type;
    std::string textureName;// TODO

protected:
    Position position;
    bool hasMoved = false;
};

#endif //RAY_CHESS_PIECE_H
