#ifndef RAY_CHESS_GAME_H
#define RAY_CHESS_GAME_H

#include <string>
#include <map>

#include "pieces/Piece.h"
#include "Board.h"
#include "raylib.h"

class Game {
public:
    const static int WINDOW_WIDTH = 640;
    const static int WINDOW_HEIGHT = 640;
    const static int CELL_SIZE = 640 / 8;

    Game();
    ~Game();

    void Run();

private:
    void LoadTextures();
    void InitBoard();
    void HandleInput();
    bool IsValidMove(const Position& move);
    void DoMove(const Position& move);

    void RenderBackground();
    void RenderPieces();
    void RenderMovesSelectedPiece();

    Board board;
    std::map<std::string, Texture> textures;

    Piece::PIECE_COLOR turnColor = Piece::PIECE_WHITE;
    Piece* selectedPiece = nullptr;
    std::vector<Position> possibleMoves;
};

#endif //RAY_CHESS_GAME_H
