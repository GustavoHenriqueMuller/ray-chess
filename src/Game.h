#ifndef RAY_CHESS_GAME_H
#define RAY_CHESS_GAME_H

#include <string>
#include <map>

#include "pieces/Piece.h"
#include "Board.h"
#include "raylib.h"
#include "Move.h"

class Game {
public:
    const static int WINDOW_WIDTH = 640;
    const static int WINDOW_HEIGHT = 640;
    const static int CELL_SIZE = 640 / 8;

    constexpr static Color LIGHT_SHADE = Color{235, 236, 208, 255};
    constexpr static Color DARK_SHADE = Color{119, 149, 86, 255};

    Game();
    ~Game();

    void Run();

private:
    void LoadTextures();
    void HandleInput();
    Move* GetMoveAtPosition(const Position& position);

    void DoMove(const Move& move);
    void DoShortCastling(const Move& move);
    void DoLongCastling(const Move& move);
    void MovePiece(Piece* piece, const Move& move);

    Board board;
    std::map<std::string, Texture> textures;

    Piece::COLOR turn = Piece::COLOR::C_WHITE;
    Piece* selectedPiece = nullptr;
    std::vector<Move> possibleMoves;
};

#endif //RAY_CHESS_GAME_H
