#ifndef RAY_CHESS_GAME_H
#define RAY_CHESS_GAME_H

#include <string>
#include <map>

#include "pieces/Piece.h"
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

    void RenderBackground();
    void RenderPieces();

    Piece* board[8][8];
    std::map<std::string, Texture> textures;
};

#endif //RAY_CHESS_GAME_H
